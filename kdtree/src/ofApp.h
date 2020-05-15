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
		//void dragEvent2(ofDragInfo dragInfo);
		void drawBox(const Box &box);
		
		void loadVbo();
		Box meshBounds(const ofMesh &);
		
		//camera setup 
		ofEasyCam cam;
		ofCamera *theCam = NULL;
		ofCamera topCam;
		ofCamera trackCam;
		ofCamera onBoardCam;
		ofCamera personCam;
		
		ofLight light;
		ofImage backgroundImage;

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
		ParticleEmitter landingEmitter;
		ParticleEmitter explosion;

		// adding forces
		//
		TurbulenceForce *turbForce;
		GravityForce *gravityForce;
		ImpulseRadialForce *radialForce;
		TurbulenceForce tur1, tur2;

		
		bool bBackgroundLoaded = false;
		bool bLanderLoaded = false;
		bool bWireFrame = false;
		bool bModelLoaded = false;
		bool bPlaneLoaded = false;
		bool bPersonCam = false;
		bool bOnBoardCam = false;

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
		int life;

		// sound
		ofSoundPlayer landerMvmt;
		ofSoundPlayer landingSound;
		ofSoundPlayer explosionSound;
		ofSoundPlayer backgroundSound;

		// lighting variables 
		ofLight landingArea1, landingArea2, landingArea3, areaLight, sunlight;
		ofPlanePrimitive plane;
		ofMaterial planeMaterial;

		bool landing = false;
		
		ParticleEmitter emitter;
		GravityForce grav;
		ImpulseForce impulse;  // test for collisions;
		float groundPlaneWidth = 100;
		float groundPlaneHeight = 100;
		float restitution = .85; 
		void checkCollisions();

		// textures
		ofTexture  particleTex;

		// shader variables
		ofVbo vbo;
		ofShader shader;
		float scale;
		float altitude;
		bool checkAltitude;
		bool gameOver;
		bool startGame;
		float gameStart;
		float gameTime;
		bool restart;

		ofMesh areaMesh1, areaMesh2, areaMesh3;
		vector<ofMesh> areaMeshes;
		ofIcoSpherePrimitive area1, area2, area3;
		vector<ofIcoSpherePrimitive> areas;
		bool insideSphere(ofVec3f point, ofIcoSpherePrimitive sphere);
		void checkLanding();
		
};
