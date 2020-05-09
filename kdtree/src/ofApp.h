#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "box.h"
#include "ray.h"
#include "KdTree.h"




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
		void toggleSelectTerrain();
		void setCameraTarget();
		glm::vec3 getMousePointOnPlane();
		glm::vec3 getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm);
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		void dragEvent2(ofDragInfo dragInfo);
		void drawBox(const Box &box);
		Box meshBounds(const ofMesh &);
		
		ofEasyCam cam;
		ofLight light;
		ofImage backgroundImage;
		ofCamera *theCam = NULL;
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
		bool bHide = false;
		
		ParticleEmitter lemEmit;
		ParticleSystem lemsys;
		//GravityForce gray;

		TurbulenceForce tur1, tur2;

		
		bool bBackgroundLoaded = false;
		bool bLanderLoaded = false;
		bool bWireFrame = false;
		bool bModelLoaded = false;
		bool bPlaneLoaded = false;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		ofxAssimpModelLoader terrain, lander;
		
		Box boundingBox;
		Box landerBounds;
		bool bPointSelected;
		bool bDisplayPoints;
		bool bTerrainSelected;
		bool bLanderSelected = false;
		bool bInDrag = false; 
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;
		KdTree kdtree;
		vector<Box> bboxList;
		glm::vec3 mouseDownPos;
		glm::vec3 mouseLastPos;


		ofxIntSlider level;
		bool bdrawbox = false;
		float starttime;
		float endtime;
		TreeNode hitNode;
		bool bcheckhit = false;
		bool intersect = false;
		vector<Box> boxHitList;
		bool collision = false;

		// sound
		ofSoundPlayer landerMvmt;

		// lighting variables 
		ofLight landingArea1, landingArea2, landingArea3, areaLight, sunlight;
		ofPlanePrimitive plane;
		ofMaterial planeMaterial;
};
