#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "box.h"
#include "ray.h"



class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
    glm::vec3 getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm);
    void drawBox(const Box &box);
    Box meshBounds(const ofMesh &);
		
		
		ofEasyCam cam;
		ofxAssimpModelLoader lander, terrain;
		ofLight light;
		ofImage backgroundImage;
		ofCamera *theCam;
		ofCamera topCam;
		
		float angle;
		ofVec3f heading;
		ofVec3f prevPos;
		ofVec3f transPos;
		Particle lem;

		ofxFloatSlider step;
		ofxFloatSlider radius;
		ofxFloatSlider rate;
		ofxPanel gui;
		
		ParticleEmitter lemEmit;
		ParticleSystem lemsys;
		//GravityForce gray;

		TurbulenceForce tur1, tur2;
    
    glm::vec3 mouseDownPos;
    glm::vec3 mouseLastPos;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
	
		bool bBackgroundLoaded = false;
		bool bLanderLoaded = false;
    bool bLanderSelected = false;
    bool bTerrainSelected;
    bool bInDrag = false;
    
    Box boundingBox;
    Box landerBounds;
    
    // sound
    ofSoundPlayer landerMvmt;
    
    // lighting variables 
    ofLight landingArea1, landingArea2, landingArea3, areaLight, sunlight;
    ofPlanePrimitive plane;
    ofMaterial planeMaterial;

};
