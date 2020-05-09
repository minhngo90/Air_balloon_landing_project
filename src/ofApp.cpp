
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


	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
    bTerrainSelected = true;

	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	cam.disableMouseInput();

	topCam.setNearClip(.1);
	topCam.setFov(65.5);
	topCam.setPosition(0, 25, 0);
	topCam.lookAt(glm::vec3(0, 0, 0));

	// set current camera;
	//
	theCam = &cam;

	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableDepthTest();
    ofEnableLighting();
    
    // add in lighting for terrain area
    
    // spot light for first landnig area
    landingArea1.setup();
    landingArea1.enable();
    landingArea1.setSpotlight();
    landingArea1.setScale(.05);
    landingArea1.setSpotlightCutOff(15);
    landingArea1.setAttenuation(1, .001, .001);
    landingArea1.setAmbientColor(ofFloatColor(1, 0, 0));
    landingArea1.setDiffuseColor(ofColor(255, 0, 0));
    landingArea1.setSpecularColor(ofColor(255, 0, 0));
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
    landingArea3.setAmbientColor(ofFloatColor(0, 0, 1));
    landingArea3.setDiffuseColor(ofColor(0, 0, 255));
    landingArea3.setSpecularColor(ofColor(0, 0, 255));
    landingArea3.rotate(-90, ofVec3f(1, 0, 0));
    landingArea3.setPosition(-1, 5, -4);
    
    // create a directional sunlight onto the entire area 
    areaLight.setup();
    areaLight.enable();
    areaLight.setPointLight();
    areaLight.setScale(.05);
    areaLight.setAmbientColor(ofColor(0,191,255));
    areaLight.setDiffuseColor(ofColor(0,191,255));
    areaLight.setSpecularColor(ofColor(0,191,255));
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
	bBackgroundLoaded = backgroundImage.load("images/starry_night4.jpg");
//    cout << ofGetWindowWidth() << " " << ofGetWindowHeight() << endl;


	// setup rudimentary lighting 
	//
	initLightingAndMaterials();


	// load lander model
	//
	if (lander.loadModel("geo/BalloonRover.obj")) {
		lander.setScaleNormalization(false);
//		lander.setScale(.5, .5, .5);
		lander.setRotation(0, -180, 1, 0, 0);
		lander.setPosition(lem.position.x, lem.position.y, lem.position.z);
//        lander.setPosition(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z);
		bLanderLoaded = true;
	}
	else {
		cout << "Error: Can't load model" << "geo/lander.obj" << endl;
		ofExit(0);
	}
    
    // load the terrain and a bounding box
    terrain.loadModel("geo/EarthModel.obj");
    terrain.setScaleNormalization(false);

    boundingBox = meshBounds(terrain.getMesh(0));

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
    
    // sounds for thruster movement
    landerMvmt.load("sounds/landerMvmt.mp3");
    
    // Set up GUI
    gui.setup();
    gui.add(level.setup("level", 10, 1, 40));
    starttime = ofGetElapsedTimeMillis();
    
    // create KdTree for terrain
    //
    kdtree.create(terrain.getMesh(0), 10);
    endtime = ofGetElapsedTimeMillis();
    cout << "building tree time :" << (endtime - starttime) / 1000 << "s." << endl;
    
}


void ofApp::update() {
	
    if (bInDrag) {
//        lemsys.forces.clear();
//        lemsys.particles.clear();
//        ofVec3f pos = lemsys.particles[0].position;
//        lander.setPosition(pos.x, pos.y, pos.z);
        return;
    }
//    else {
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
//    }
	
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
		backgroundImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		ofEnableDepthTest();
		ofPopMatrix();
	}

	ofSetColor(ofColor::white);
	

	theCam->begin();

	ofPushMatrix();
    
    // draw the lights
//    landingArea1.draw();
//    landingArea2.draw();
//    landingArea3.draw();
//    areaLight.draw();
//    sunlight.draw();

		// draw a reference grid
		//
//		ofPushMatrix();
//		ofRotate(90, 0, 0, 1);
//		ofSetLineWidth(1);
//		ofSetColor(ofColor::dimGray);
//		ofDisableLighting();
////		ofDrawGridPlane();
//		ofPopMatrix();

	if (bWireframe) {                    // wireframe mode  (include axis)
            ofDisableLighting();
            ofSetColor(ofColor::slateGray);
            terrain.drawWireframe();
            if (bLanderLoaded) {
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
                
                // if hit then change color of box to red if not then keep it as white
                if (bLanderSelected) {
                    ofSetColor(ofColor::red);
                }
                else {
                    ofSetColor(ofColor::white);
                }
    
                drawBox(bounds);
            }
            if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
        }

	//lemsys.draw();
	lemEmit.draw();

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
	}

	
//	ofPopMatrix();
    
    ofNoFill();
    ofSetColor(ofColor::white);
//    drawBox(boundingBox);
    
    if (bdrawbox) {
        // debug - check first node to make sure bbox is correct
        //
        //drawBox(kdtree.root.box);

        //draw all the box
        kdtree.draw(level, 0);

        
    }

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
            bInDrag = true;
            lemsys.particles[0].position = lander.getPosition();
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
		//lem.forces += ofVec3f(0,10,0);
		
		lemsys.addForce(new ThrusterForce(ofVec3f(0, 10, 0)));
		lemEmit.start();
            landerMvmt.play();
		break;
	case 's':     // spacefraft thrust DOWN
		//lem.forces -= ofVec3f(0, 10, 0);
		
		lemsys.addForce(new ThrusterForce(ofVec3f(0, -10, 0)));
		lemEmit.start();
            landerMvmt.play();
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
            landerMvmt.play();
		break;
	case OF_KEY_DOWN:   // move backward
		//lem.forces -= ofVec3f(heading).rotate(angle, ofVec3f(0, 1, 0));
		lemsys.addForce(new ThrusterForce(-ofVec3f(heading).rotate(angle, ofVec3f(0, 1, 0))));
		lemEmit.start();
            landerMvmt.play();
		break;
	case OF_KEY_LEFT:   // move left
		//lem.forces -= ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0));
		lemsys.addForce(new ThrusterForce(-ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0))));
		lemEmit.start();
            landerMvmt.play();
		break;
	case OF_KEY_RIGHT:   // move right
		//lem.forces += ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0));
		lemsys.addForce(new ThrusterForce(ofVec3f(heading).rotate(angle-90, ofVec3f(0, 1, 0))));
		lemEmit.start();
            landerMvmt.play();
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
        case 'h':
            bInDrag = false;
            // set position of particle to lander
	case 'w':     
		lemEmit.stop();
		break;
	case 's':     
		lemEmit.stop();
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

    //  implement you code here to select the rover
    //  if Selected, draw box in a different color
//    lemEmit.stop();
    
    glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
    glm::vec3 rayDir = glm::normalize(p - theCam->getPosition());
    
    // compute bounds
    glm::vec3 min = lander.getSceneMin() + lander.getPosition();
    glm::vec3 max = lander.getSceneMax() + lander.getPosition();
    Box bound = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
    
    // test intersection with ray
    if (bound.intersect(Ray(Vector3(p.x, p.y, p.z), Vector3(rayDir.x, rayDir.y, rayDir.z)), 0, 10000)) {
        cout << "HIT!" << endl;
        mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
//        mouseDownPos = getMousePointOnPlane(ofVec3f(lem.position.x, lem.position.y, lem.position.z), cam.getZAxis());
        mouseLastPos = mouseDownPos;
        bInDrag = true;
        bLanderSelected = true;
    }
    else {
        cout << "MISS!" << endl;
        bLanderSelected = false;
    }
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



//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
   //
        //  implement your code here to drag the lander around
        
        // if moving camera, don't allow mouse interaction//
        if (cam.getMouseInputEnabled()) return;
        
        // check if box is clicked
        if (bLanderSelected && bInDrag) {
//            getMousePointOnPlane();
            lemEmit.start();
            landerMvmt.play();
            glm::vec3 landerPos = lander.getPosition();
            glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
            glm::vec3 delta = mousePos - mouseLastPos;
            cout << "Initial Position: " << landerPos << endl;
            cout << "Change in position: " << delta << endl;
            landerPos += delta;
            cout << "Lander Position: " << landerPos << endl;
//            lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
            lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
            mouseLastPos = mousePos;
        }
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

//  intersect the mouse ray with the plane normal to the camera
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
    // Setup our rays
        //
        glm::vec3 origin = theCam->getPosition();
        glm::vec3 camAxis = theCam->getZAxis();
        glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
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
