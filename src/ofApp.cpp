
//--------------------------------------------------------------
//
//  03.29.20
//
//  CS 134 Midterm - Spring 2020
// 
//   author:  Minh Ngo   

#include "ofApp.h"

//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup() {


	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;

	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	cam.disableMouseInput();

	topCam.setNearClip(.1);
	topCam.setFov(65.5);
	topCam.setPosition(0, 15, 0);
	topCam.lookAt(glm::vec3(0, 0, 0));

	// set current camera;
	//
	theCam = &cam;

	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();

	// load BG image
	//
	bBackgroundLoaded = backgroundImage.load("images/starfield-plain.jpg");


	// setup rudimentary lighting 
	//
	initLightingAndMaterials();


	// load lander model
	//
	if (lander.loadModel("geo/lander.obj")) {
		lander.setScaleNormalization(false);
		lander.setScale(.5, .5, .5);
		lander.setRotation(0, -180, 1, 0, 0);
		lander.setPosition(lem.position.x, lem.position.y, lem.position.z);
		
		bLanderLoaded = true;
	}
	else {
		cout << "Error: Can't load model" << "geo/lander.obj" << endl;
		ofExit(0);
	}

	// setup LEM
	//
	
	lem.acceleration.set(0, 0, 0);
	lem.forces.set(0, 0, 0);
	lem.velocity.set(0, 0, 0);
	lem.position.set(0, 0, 0);
	lem.lifespan =10000;
	lem.radius = .5;
	lem.damping = .99;
	lem.mass = 1;
	lemsys.add(lem);
	//lemsys.addForce(new GravityForce(ofVec3f(0, -10, 0)));
	//lemsys.addForce(new ThrusterForce(ofVec3f(0, 10, 0)));


	//set up thurster emmitter
	lemEmit.type = DiscEmitter; //Disc Emitter type
	lemEmit.radius = .2;
	lemEmit.setVelocity(ofVec3f(0, -2, 0));
	lemEmit.setParticleRadius(0.01);
	lemEmit.setGroupSize(50);
	
	lemEmit.setRate(100);
	lemEmit.setLifespan(1);
	lemEmit.sys->addForce(new GravityForce(ofVec3f(0, -5, 0)));
	lemEmit.sys->addForce(new TurbulenceForce(ofVec3f(-2, -1, -3), ofVec3f(1, 2, 5)));
	
	
	
	angle = 0;
	heading = 20*ofVec3f(0,0,-1);
		
}





void ofApp::update() {
	
	
	// landing on the plane
	
	if (lem.position.y < 0) {
		lem.position.y = 0;
	}
	
	//lem.integrate();
	
	lemsys.update();
	
	
	//update emmitters
	lemEmit.update();
	lemEmit.setPosition(lemsys.particles[0].position);
	
	// set position of lander as single lem particle
	lander.setPosition(lemsys.particles[0].position.x, lemsys.particles[0].position.y, lemsys.particles[0].position.z);
	
}

//--------------------------------------------------------------
void ofApp::draw() {

	// draw background image
	//
	if (bBackgroundLoaded) {
		ofPushMatrix();
		ofDisableDepthTest();
		ofSetColor(50, 50, 50);
		ofScale(2, 2);
		backgroundImage.draw(-200, -100);
		ofEnableDepthTest();
		ofPopMatrix();
	}

	ofSetColor(ofColor::white);
	

	theCam->begin();

	ofPushMatrix();

		// draw a reference grid
		//
		ofPushMatrix();
		ofRotate(90, 0, 0, 1);
		ofSetLineWidth(1);
		ofSetColor(ofColor::dimGray);
		ofDisableLighting();
		ofDrawGridPlane();
		ofPopMatrix();

	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		if (bLanderLoaded) {

			//  Note this is how to rotate LEM model about the "Y" or "UP" axis
			//
			lander.setRotation(0, angle, 0, 1, 0);    
			lander.drawWireframe();
		}
	}
	else {
		ofEnableLighting();              // shaded mode
		if (bLanderLoaded) {
			lander.setRotation(0, angle, 0, 1, 0);
			lander.drawFaces();

		}
	}

	//lemsys.draw();
	lemEmit.draw();

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
	}

	
	ofPopMatrix();

	theCam->end();

	// draw screen data
	//
	string str;
	str += "Frame Rate: " + std::to_string(ofGetFrameRate());
	ofSetColor(ofColor::white);
	ofDrawBitmapString(str, ofGetWindowWidth() - 170, 15);

	string str2;
	str2 += "Altitide (AGL): " + std::to_string(lander.getPosition().y);
	ofSetColor(ofColor::white);
	ofDrawBitmapString(str2, 5, 15);

	
}


// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'P':
	case 'p':
		break;
	case 'r':
		cam.reset();
		break;
	case 'g':
		savePicture();
		break;
	case 't':
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'm':
		toggleWireframeMode();
		break;
	case 'd':     // rotate spacecraft clockwise (about Y (UP) axis)
		angle += 1;
		break;
	case 'a':     // rotate spacecraft counter-clockwise (about Y (UP) axis)
		angle -= 1;
		break;
	case 'w':     // spacecraft thrust UP
		//lem.forces += ofVec3f(0,10,0);
		
		lemsys.addForce(new ThrusterForce(ofVec3f(0, 10, 0)));
		
		lemEmit.start();
		break;
	case 's':     // spacefraft thrust DOWN
		//lem.forces -= ofVec3f(0, 10, 0);
		
		lemsys.addForce(new ThrusterForce(ofVec3f(0, -10, 0)));
		lemEmit.start();
		break;
	case OF_KEY_F1:
		theCam = &cam;
		break;
	case OF_KEY_F3:
		theCam = &topCam;
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_UP:    // move forward
		//lem.forces += ofVec3f(heading).rotate(angle, ofVec3f(0, 1, 0));
		lemsys.addForce(new ThrusterForce(ofVec3f(heading).rotate(angle, ofVec3f(0, 1, 0))));
		lemEmit.start();
		break;
	case OF_KEY_DOWN:   // move backward
		//lem.forces -= ofVec3f(heading).rotate(angle, ofVec3f(0, 1, 0));
		lemsys.addForce(new ThrusterForce(-ofVec3f(heading).rotate(angle, ofVec3f(0, 1, 0))));
		lemEmit.start();
		break;
	case OF_KEY_LEFT:   // move left
		//lem.forces -= ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0));
		lemsys.addForce(new ThrusterForce(-ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0))));
		lemEmit.start();
		break;
	case OF_KEY_RIGHT:   // move right
		//lem.forces += ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0));
		lemsys.addForce(new ThrusterForce(ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0))));
		lemEmit.start();
		break;
	default:
		break;
	}
}


void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}


void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {
	
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	case 'w':     
		lemEmit.stop();
		break;
	case 's':     
		lemEmit.stop();
		break;
	case OF_KEY_UP:    // move forward
		
		lemEmit.stop();
		break;
	case OF_KEY_DOWN:   // move backward
		
		lemEmit.stop();
		break;
	case OF_KEY_LEFT:   // move left
		
		lemEmit.stop();
		break;
	case OF_KEY_RIGHT:   // move right
		
		lemEmit.stop();
		break;
	default:
		break;

	}

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {


}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}





//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ .7f, .7f, .7f, 1.0f };

	static float position[] =
	{20.0, 20.0, 20.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
//	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
//	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
