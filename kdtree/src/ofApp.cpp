
//--------------------------------------------------------------
//
//  03.29.20
//
//  CS 134 Midterm - Spring 2020
// 
//   author:  Minh Ngo   

#include "ofApp.h"
#include "Util.h"
#include <glm/gtx/intersect.hpp>

//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup() {


	bWireFrame = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
	ofSetWindowShape(1024, 768);

	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	
	

	topCam.setNearClip(.1);
	topCam.setFov(65.5);
	topCam.setPosition(0, 25, 0);
	topCam.lookAt(glm::vec3(0, 0, 0));

	// set current camera;
	//
	theCam = &cam;

	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();
	
	// add in lighting for terrain area

	// spot light for first landnig area
	landingArea1.setup();
	landingArea1.enable();
	landingArea1.setSpotlight();
	landingArea1.setScale(.05);
	landingArea1.setSpotlightCutOff(15);
	landingArea1.setAttenuation(1, .001, .001);
	landingArea1.setAmbientColor(ofFloatColor(0, 0, 1));
	landingArea1.setDiffuseColor(ofColor(0, 0, 255));
	landingArea1.setSpecularColor(ofColor(0, 0,255));
	landingArea1.rotate(-90, ofVec3f(1, 0, 0));
	landingArea1.setPosition(-9, 5, 8);

	// spot light for second landnig area
	landingArea2.setup();
	landingArea2.enable();
	landingArea2.setSpotlight();
	landingArea2.setScale(.05);
	landingArea2.setSpotlightCutOff(15);
	landingArea2.setAttenuation(1, .001, .001);
	landingArea2.setAmbientColor(ofFloatColor(0, 1, 0));
	landingArea2.setDiffuseColor(ofColor(0, 255, 0));
	landingArea2.setSpecularColor(ofColor(0, 255, 0));
	landingArea2.rotate(-90, ofVec3f(1, 0, 0));
	landingArea2.setPosition(-4, 5, 4);

	// spot light for third landnig area
	landingArea3.setup();
	landingArea3.enable();
	landingArea3.setSpotlight();
	landingArea3.setScale(.05);
	landingArea3.setSpotlightCutOff(15);
	landingArea3.setAttenuation(1, .001, .001);
	landingArea3.setAmbientColor(ofFloatColor(0, 1, 0));
	landingArea3.setDiffuseColor(ofColor(0, 255, 0));
	landingArea3.setSpecularColor(ofColor(0, 255, 0));
	landingArea3.rotate(-90, ofVec3f(1, 0, 0));
	landingArea3.setPosition(-1, 5, -4);

	// create a directional sunlight onto the entire area 
	areaLight.setup();
	areaLight.enable();
	areaLight.setPointLight();
	areaLight.setScale(.05);
	areaLight.setAmbientColor(ofColor(0, 191, 255));
	areaLight.setDiffuseColor(ofColor(0, 191, 255));
	areaLight.setSpecularColor(ofColor(0, 191, 255));
	areaLight.rotate(45, ofVec3f(0, 1, 0));
	areaLight.rotate(-45, ofVec3f(1, 0, 0));
	areaLight.setPosition(5, 15, 5);

	// create light for sun rays
	sunlight.setup();
	sunlight.enable();
	sunlight.setPointLight();
	sunlight.setScale(.05);
	sunlight.setAmbientColor(ofFloatColor(1, 0, 0));
	sunlight.setDiffuseColor(ofFloatColor(1, 1, 0));
	sunlight.setSpecularColor(ofFloatColor(1, 0, 0));
	sunlight.rotate(-45, ofVec3f(0, 1, 0));
	sunlight.rotate(45, ofVec3f(1, 0, 0));
	sunlight.setPosition(15, 15, -15);

	// load BG image
	//
	//bBackgroundLoaded = backgroundImage.load("images/starfield-plain.jpg");
	bBackgroundLoaded = backgroundImage.load("images/starry_night4.jpg");

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	// setup terrain model
	terrain.loadModel("geo/EarthModel.obj");
	terrain.setScaleNormalization(false);
	//terrain.setScale(.5, .5, .5);

	boundingBox = meshBounds(terrain.getMesh(0));

	// load lander model
	//
	if (lander.loadModel("geo/BalloonRover.obj")) {
		lander.setScaleNormalization(false);
		//lander.setScale(.5, .5, .5);
		lander.setRotation(0, -180, 1, 0, 0);
		lander.setPosition(lem.position.x, lem.position.y, lem.position.z);
		
		bLanderLoaded = true;

		// Setup our rays
		//
		glm::vec3 origin = theCam->getPosition();
		glm::vec3 camAxis = theCam->getZAxis();
		glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
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
	lemEmit.setVelocity(ofVec3f(0, 1, 0));
	lemEmit.setParticleRadius(0.01);
	lemEmit.setGroupSize(50);
	
	lemEmit.setRate(100);
	lemEmit.setLifespan(1);
	lemEmit.sys->addForce(new GravityForce(ofVec3f(0, 2, 0)));
	lemEmit.sys->addForce(new TurbulenceForce(ofVec3f(-2, -1, -3), ofVec3f(1, 2, 5)));
	
	
	angle = 0;
	heading = 1*ofVec3f(0,0,-1);

	// sounds for thruster movement
	landerMvmt.load("sounds/landerMvmt.mp3");

	//set up explosion sound of the ship
	explosionSound.load("sounds/blast.mp3");

	//set up landing sound of the ship
	landingSound.load("sounds/opening.mp3");

	 
	// Set up GUI 
	gui.setup();
	gui.add(level.setup("level", 10, 1, 40));
	cout << "Start building tree" << endl;
	starttime = ofGetElapsedTimeMillis();
	
	// create KdTree for terrain
	//
	kdtree.create(terrain.getMesh(0), 10);
	endtime = ofGetElapsedTimeMillis();
	cout << "building tree time :" << (endtime - starttime) / 1000 << "s." << endl;

	// set up the emitter forces
	//
	turbForce = new TurbulenceForce(ofVec3f(-5, -5, -5), ofVec3f(5, 5, 5));
	gravityForce = new GravityForce(ofVec3f(0, -10, 0));
	radialForce = new ImpulseRadialForce(200.0);
	radialForce->setHeight(0);
	//radialForce->set(0.5);
		
	landingEmitter.sys->addForce(turbForce);
	landingEmitter.sys->addForce(gravityForce);
	landingEmitter.sys->addForce(radialForce);
	landingEmitter.setVelocity(ofVec3f(0, 1, 0));
	landingEmitter.setOneShot(true);
	landingEmitter.setEmitterType(RadialEmitter);
	landingEmitter.setGroupSize(5000);
	landingEmitter.setPosition(lander.getPosition());
	landingEmitter.setLifespan(1);
	landingEmitter.setRate(15);
	landingEmitter.setParticleRadius(0.01);

	landing = true;

	explosion.sys->addForce(new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20)));
	explosion.sys->addForce(gravityForce);
	explosion.sys->addForce(new ImpulseRadialForce(500.0));
	explosion.setVelocity(ofVec3f(0, 100, 0));
	explosion.setOneShot(true);
	explosion.setEmitterType(RadialEmitter);
	explosion.setGroupSize(5000);
	explosion.setPosition(lander.getSceneCenter() + lander.getPosition());
	explosion.setLifespan(.5);
	explosion.setRate(5);
	explosion.setParticleRadius(.05);


	//test collision
	emitter.setRate(1);
	emitter.setOneShot(true);

	emitter.start();
	emitter.setLifespan(10);
	
	ParticleSystem *sys = emitter.sys;
	grav.set(ofVec3f(0, -1.0, 0));
	sys->addForce(&grav);
	sys->addForce(new TurbulenceForce(ofVec3f(-3, -1, -1), ofVec3f(3, 1, 1)));
}





void ofApp::update() {
	if (!bInDrag) {
		/*
		// landing on the plane
		if (lemsys.particles[0].position.y < 0) {
			lemsys.particles[0].position.y = 0;
		}
		*/
		lemsys.update();


		//update emmitters
		lemEmit.update();
		lemEmit.setPosition(ofVec3f(lemsys.particles[0].position.x, lemsys.particles[0].position.y+2, lemsys.particles[0].position.z));

		// set position of lander as single lem particle
		lander.setPosition(lemsys.particles[0].position.x, lemsys.particles[0].position.y, lemsys.particles[0].position.z);
	}

	// check box intersect of lander and terrain
	boxHitList.clear();
	if (kdtree.intersect(landerBounds, kdtree.root, boxHitList)) {
		intersect = true;
	}
	else intersect = false;

	landingEmitter.setPosition(lander.getPosition());

	// landing effect
	if (landing) {
		landingEmitter.update();
	}

	// explosion effect
	explosion.setPosition(lander.getSceneCenter() + lander.getPosition());
	explosion.update();
	

	lemEmit.start();

	// check collision
	checkCollisions();
	grav.set(ofVec3f(0, -10, 0));
	//emitter.setParticleRadius(radius);

	// get velocity from slider
	//glm::vec3 v = velocity;
	//emitter.setVelocity(v);
	
	// update objects you've created here
	emitter.update();
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
		//backgroundImage.draw(-200, -100);
		backgroundImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		ofEnableDepthTest();
		ofPopMatrix();
	}
	ofEnableDepthTest();
	

	ofSetColor(ofColor::white);
	

	theCam->begin();

	ofPushMatrix();

	// draw the lights
    //landingArea1.draw();
    //landingArea2.draw();
    //landingArea3.draw();
    //areaLight.draw();
    //sunlight.draw();

		// draw a reference grid
		//
	/*
		ofPushMatrix();
		ofRotate(90, 0, 0, 1);
		ofSetLineWidth(1);
		ofSetColor(ofColor::dimGray);
		ofDisableLighting();
		ofDrawGridPlane();
		ofPopMatrix();
		*/
	if (bWireFrame) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		terrain.drawWireframe();
		if (bLanderLoaded) {

			//  Note this is how to rotate LEM model about the "Y" or "UP" axis
			//
			
			lander.setRotation(0, angle, 0, 1, 0);    
			lander.drawWireframe();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		terrain.drawFaces();
		if (bLanderLoaded) {
			lander.setRotation(0, angle, 0, 1, 0);
			lander.drawFaces();
			if (!bTerrainSelected) drawAxis(lander.getPosition());

			ofVec3f min = lander.getSceneMin() + lander.getPosition();
			ofVec3f max = lander.getSceneMax() + lander.getPosition();

			Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

			landerBounds = bounds;

			// set color of bounding box based on selection status
			//
			if (bLanderSelected) ofSetColor(ofColor::red);
			else ofSetColor(ofColor::white);

			drawBox(bounds);
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}

	//lemsys.draw();
	ofSetColor(ofColor::white);
	lemEmit.draw();

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		terrain.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(hitNode.box.center().x(), hitNode.box.center().y(), hitNode.box.center().z(), .5);
	}

	if (intersect) {
		for (int i = 0; i < boxHitList.size(); i++) {
			drawBox(boxHitList[i]);
		}		
	}
	emitter.draw();

	ofNoFill();
	ofSetColor(ofColor::white);
	//	drawBox(boundingBox);

	if (bdrawbox) {
		// debug - check first node to make sure bbox is correct
		//
		//drawBox(kdtree.root.box);

		//draw all the box
		kdtree.draw(level, 0);

		
	}

	//draw only leaf nodes
	//kdtree.drawLeafNodes(kdtree.root);
	
	// draw landing effect here..
	//
	if (landing) {
		ofSetColor(ofColor::white);
		landingEmitter.draw();
	}
	
	ofSetColor(ofColor::white);
	explosion.draw();
	
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

	ofDisableDepthTest();
	//draw gui pannel
	if (bHide) {
		gui.draw();
	}

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
		bHide = !bHide;
		break;
	case 'P':
	case 'p':
		// kdtree ray collision
		bcheckhit = !bcheckhit;
		break;
	case 'r':
		cam.reset();
		break;
	case 'g':
		savePicture();
		break;
	case 't':
		landingEmitter.sys->reset();
		landingEmitter.start();
		landingSound.play();
		break;
	case 'u':
		bdrawbox = !bdrawbox;
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
		lemsys.addForce(new ThrusterForce(ofVec3f(0, 2, 0)));
		lemEmit.sys->addForce(new ThrusterForce(ofVec3f(0, 1, 0)));
		//lemEmit.start();
		break;
	case 's':     // spacefraft thrust DOWN
				
		lemsys.addForce(new ThrusterForce(ofVec3f(0, -2, 0)));
		lemEmit.sys->addForce(new ThrusterForce(ofVec3f(0, 1, 0)));
		//lemEmit.start();
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
		emitter.start();
		break;
	case OF_KEY_SHIFT:
		explosion.sys->reset();
		explosion.start();
		explosionSound.play();
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_UP:    // move forward
		
		lemsys.addForce(new ThrusterForce(ofVec3f(heading).rotate(angle, ofVec3f(0, 1, 0))));
		lemEmit.start();
		landerMvmt.play();
		break;
	case OF_KEY_DOWN:   // move backward
		
		lemsys.addForce(new ThrusterForce(-ofVec3f(heading).rotate(angle, ofVec3f(0, 1, 0))));
		lemEmit.start();
		landerMvmt.play();
		break;
	case OF_KEY_LEFT:   // move left
		
		lemsys.addForce(new ThrusterForce(-ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0))));
		lemEmit.start();
		landerMvmt.play();
		break;
	case OF_KEY_RIGHT:   // move right
		
		lemsys.addForce(new ThrusterForce(ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0))));
		lemEmit.start();
		landerMvmt.play();
		break;
	default:
		break;
	}
}


void ofApp::toggleWireframeMode() {
	bWireFrame = !bWireFrame;
}


void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
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
		landerMvmt.stop();
		break;
	case 's':     
		lemEmit.stop();
		landerMvmt.stop();
		break;
	case OF_KEY_UP:    // move forward
		
		lemEmit.stop();
		landerMvmt.stop();
		break;
	case OF_KEY_DOWN:   // move backward
		
		lemEmit.stop();
		landerMvmt.stop();
		break;
	case OF_KEY_LEFT:   // move left
		
		lemEmit.stop();
		landerMvmt.stop();
		break;
	case OF_KEY_RIGHT:   // move right
		
		lemEmit.stop();
		landerMvmt.stop();
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

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;



	glm::vec3 p = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 rayDir = glm::normalize(p - theCam->getPosition());
	
	if (bcheckhit) {
		//check ray collision of the mouse with the leaf node
		if (kdtree.intersect(Ray(Vector3(p.x, p.y, p.z), Vector3(rayDir.x, rayDir.y, rayDir.z)), kdtree.root, hitNode)) {
			bPointSelected = true;
		}
		else bPointSelected = false;
	} else bPointSelected = false;
	
	

	if (bLanderLoaded) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		// compute bounds
		//
		glm::vec3 min = lander.getSceneMin() + lander.getPosition();
		glm::vec3 max = lander.getSceneMax() + lander.getPosition();
		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		// test intersection with ray
		//
	//	bool intersect(const Ray &, float t0, float t1) const;

		if (bounds.intersect(Ray(Vector3(p.x, p.y, p.z), Vector3(rayDir.x, rayDir.y, rayDir.z)), 0, 1000)) {
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
			//cout << "bInDrag: " << bInDrag << endl;
		}
		else {
			bLanderSelected = false;
		}
	}
	//  implement you code here to select the rover
	//  if Selected, draw box in a different color


}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	// 
//  implement your code here to drag the lander around
// if moving camera, don't allow mouse interaction
//
	if (cam.getMouseInputEnabled()) return;

	if (bLanderSelected && bInDrag) {
		//cout << "dragged bInDrag: " << bInDrag << endl;
		glm::vec3 landerPos = lander.getPosition();
		//cout << landerPos << endl;
		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		
		glm::vec3 delta = mousePos - mouseLastPos;
		//cout << delta << endl;
		landerPos += delta;
		//cout << landerPos << endl;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

		//ofVec3f min = lander.getSceneMin() + lander.getPosition();
		//ofVec3f max = lander.getSceneMax() + lander.getPosition();

		

	}

	if (bcheckhit) {
		// check ray collision with the leaf node when dragging the mouse 
		glm::vec3 p = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 rayDir = glm::normalize(p - theCam->getPosition());
		if (kdtree.intersect(Ray(Vector3(p.x, p.y, p.z), Vector3(rayDir.x, rayDir.y, rayDir.z)), kdtree.root, hitNode)) {
			bPointSelected = true;
		}
		else bPointSelected = false;
	} else bPointSelected = false;
	

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bLanderSelected = false;
	bInDrag = false;
	landerMvmt.stop();
	lemEmit.stop();
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



//draw a box from a "Box" class  
//
void ofApp::drawBox(const Box &box) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
	float w = size.x();
	float h = size.y();
	float d = size.z();
	ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
	int n = mesh.getNumVertices();
	ofVec3f v = mesh.getVertex(0);
	ofVec3f max = v;
	ofVec3f min = v;
	for (int i = 1; i < n; i++) {
		ofVec3f v = mesh.getVertex(i);

		if (v.x > max.x) max.x = v.x;
		else if (v.x < min.x) min.x = v.x;

		if (v.y > max.y) max.y = v.y;
		else if (v.y < min.y) min.y = v.y;

		if (v.z > max.z) max.z = v.z;
		else if (v.z < min.z) min.z = v.z;
	}
	return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

void ofApp::setCameraTarget() {

}

//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{ 5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
}

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		//	lander.setScale(.5, .5, .5);
		lander.setPosition(0, 0, 0);
		//		lander.setRotation(1, 180, 1, 0, 0);
		cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(KdTree::meshBounds(lander.getMesh(i)));
		}
		// We want to drag and drop a 3D object in space so that the model appears 
		// under the mouse pointer where you drop it !
		//
		// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
		// once we find the point of intersection, we can position the lander/lander
		// at that location.
		//

		// Setup our rays
		//
		glm::vec3 origin = theCam->getPosition();
		glm::vec3 camAxis = theCam->getZAxis();
		glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}


//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//

glm::vec3 ofApp::getMousePointOnPlane() {
	// Setup our rays
	//
	glm::vec3 origin = theCam->getPosition();
	glm::vec3 camAxis = theCam->getZAxis();
	glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}


glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}



//  This a very simple function to check for collision on the ground plane at (0,0,0)
//  If the partical position.y value is smaller than it's radius, we will assume
//  it's has gone through the plane and we apply a simple impulse function
//  resolve it..
//
void ofApp::checkCollisions() {

	// for each particle, determine if we hit the groud plane.
	//
	for (int i = 0; i < emitter.sys->particles.size(); i++) {

		// only bother to check for descending particles.
		//
		ofVec3f vel = emitter.sys->particles[i].velocity; // velocity of particle
		if (vel.y >= 0) break;                             // ascending;

		ofVec3f pos = emitter.sys->particles[i].position;

		if (pos.y < emitter.sys->particles[i].radius) {

			// apply impulse function
			//
			ofVec3f norm = ofVec3f(0, 1, 0);  // just use vertical for normal for now
			ofVec3f f = (restitution + 1.0)*((-vel.dot(norm))*norm);
			emitter.sys->particles[i].forces += ofGetFrameRate() * f;
		}
	}
}
