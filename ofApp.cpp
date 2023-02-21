#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	auto seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
	ofColor color;

	this->face.clear();
	this->line.clear();

	float phi_deg_step = 4;
	float theta_deg_step = 4;
	float threshold_1 = 0.4;
	float threshold_2 = 0.6;

	float R = 240;
	for (int r = 10; r <= 60; r += 10) {

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

				auto noise_value = ofNoise(glm::vec4(this->make_point(R, 30, theta_deg, phi_deg) * 0.0085, r * 0.005 + ofGetFrameNum() * 0.02));
				if (noise_value <= threshold_1 || noise_value >= threshold_2) { continue; }

				auto noise_1 = ofNoise(glm::vec4(this->make_point(R, 30, theta_deg - theta_deg_step, phi_deg) * 0.0085, r * 0.005 + ofGetFrameNum() * 0.02));
				auto noise_2 = ofNoise(glm::vec4(this->make_point(R, 30, theta_deg, phi_deg + phi_deg_step) * 0.0085, r * 0.005 + ofGetFrameNum() * 0.02));
				auto noise_3 = ofNoise(glm::vec4(this->make_point(R, 30, theta_deg, phi_deg - phi_deg_step) * 0.0085, r * 0.005 + ofGetFrameNum() * 0.02));
				auto noise_4 = ofNoise(glm::vec4(this->make_point(R, 30, theta_deg + theta_deg_step, phi_deg) * 0.0085, r * 0.005 + ofGetFrameNum() * 0.02));

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));

				this->face.addVertices(vertices);

				color = ofColor(0);
				for (int i = 0; i < 4; i++) {

					this->face.addColor(color);
				}

				color.setHsb(ofMap(r, 10, 60, 0, 255), 150, 255);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

				if (noise_1 <= threshold_1 || noise_1 >= threshold_2) {

					this->line.addVertex(vertices[0]);
					this->line.addVertex(vertices[2]);

					this->line.addColor(color);
					this->line.addColor(color);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_2 <= threshold_1 || noise_2 >= threshold_2) {

					this->line.addVertex(vertices[2]);
					this->line.addVertex(vertices[3]);

					this->line.addColor(color);
					this->line.addColor(color);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_3 <= threshold_1 || noise_3 >= threshold_2) {

					this->line.addVertex(vertices[0]);
					this->line.addVertex(vertices[1]);

					this->line.addColor(color);
					this->line.addColor(color);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_4 <= threshold_1 || noise_4 >= threshold_2) {

					this->line.addVertex(vertices[1]);
					this->line.addVertex(vertices[3]);

					this->line.addColor(color);
					this->line.addColor(color);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);

				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.333333333333333 * 6);

	this->line.draw();
	this->face.draw();

	this->cam.end();

	/*
	ostringstream os;
	os << setw(4) << setfill('0') << ofGetFrameNum() + 1;
	ofImage image;
	image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	image.saveImage("image/img_" + os.str() + ".jpg");
	if (ofGetFrameNum() + 1 >= 30 * 18) {

		std::exit(1);
	}

	// > ffmpeg -i img_%04d.jpg -r 30 out.mp4

	*/

}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// ���w�f�b�T������ �`���Ċy���ސ��w�����@P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}