#include <stdlib.h>
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include "build/tinyxml2.h"

//sda
using namespace tinyxml2;

void desenhar_cone(std::string modo_desenho);
void desenhar_Caixa();
void desenhar_Plano();
void desenhar_Esfera();

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX=0.0, camY=0.0, camZ=15.0;


void spherical2Cartesian() {
	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}
typedef struct vertice {
	float x;
	float y;
	float z;
};

vertice *conj_vertices;
int controlador_vertices;

typedef struct cone {

	int n_vertices;
	int div_horizontais;
	int div_verticais;
	
	vertice *conjunto_vertices;
};

typedef struct esfera {

	int n_vertices;
	int div_horizontais;
	int div_verticais;

	vertice *conjunto_vertices;
};


typedef struct plano {

	vertice arrayVertice[4];
};



typedef struct caixa {

	int n_vertices;
	int divisoes;

	vertice *conjunto_vertices;
};

esfera esfera_temp;
plano plano_temp;
caixa caixa_temp;
cone cone_temp;
XMLDocument cena_temp;



void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void renderScene(void) {
	
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);


	// put drawing instructions here
    //glPointSize(5);             //and this one

	//transforamçoes


	//teste
	//glColor3b(50, 50, 50);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	desenhar_Caixa();
	glTranslatef(6,0,0);
	desenhar_Plano();
	glTranslatef(-14, 0, 0);
	desenhar_cone("triangulos");
	glTranslatef(-8,4,0);
	desenhar_Esfera();

	// End of frame
	glutSwapBuffers();
}


void ler_Plano(std::string ficheiro) {
	std::ifstream stream_leitura;
	stream_leitura.open(ficheiro);
	 
	std::string xString, yString, zString, line;
	float vertice_x, vertice_y, vertice_z;
	int i = 0,n_vertices;

	if (getline(stream_leitura, line)) {
		n_vertices = stoi(line);
	}

	//ler vertices
	while (n_vertices > 0) {
		if (!getline(stream_leitura, line)) break;
		std::stringstream ss(line);
		getline(ss, xString, ',');
		getline(ss, yString, ',');
		getline(ss, zString, ',');

		vertice_x = std::stof(xString);
		vertice_y = std::stof(yString);
		vertice_z = std::stof(zString);

		vertice v;
		v.x = vertice_x;
		v.y = vertice_y;
		v.z = vertice_z;
		plano_temp.arrayVertice[i]= v;
		i++;
		n_vertices--;
	}



}
void ler_Caixa(std::string ficheiro) {
	std::ifstream stream_leitura;
	stream_leitura.open(ficheiro);

	std::string xString, yString, zString, line;
	float vertice_x, vertice_y, vertice_z;
	int i = 0;
	int divisoes, n_vertices;

	//ler divisoes->numero de vertices
	if (getline(stream_leitura, line)) {
		divisoes = stoi(line);
	}

	if (getline(stream_leitura, line)) {
		n_vertices = stoi(line);
	}

	//alocar array de vertices e inicializar caixa com divisoes
	caixa_temp.conjunto_vertices = (vertice*)malloc(n_vertices * sizeof(vertice));
	caixa_temp.divisoes = divisoes;
	caixa_temp.n_vertices = n_vertices;

	//ler vertices

	while (n_vertices > 0) {
		if (!getline(stream_leitura, line)) break;
		std::stringstream ss(line);
		getline(ss, xString, ',');
		getline(ss, yString, ',');
		getline(ss, zString, ',');

		vertice_x = std::stof(xString);
		vertice_y = std::stof(yString);
		vertice_z = std::stof(zString);

		vertice v;
		v.x = vertice_x;
		v.y = vertice_y;
		v.z = vertice_z;
		caixa_temp.conjunto_vertices[i] = v;
		i++;
		n_vertices--;
	}



}

void ler_Cone(std::string ficheiro) {
	std::ifstream stream_leitura;
	stream_leitura.open(ficheiro);
	/*std::ofstream stream_escrita;
	stream_escrita.open("debugcone.txt");*/


	std::string xString, yString, zString, line;
	float vertice_x, vertice_y, vertice_z;
	int i = 0;
	int div_verticais,div_horizontais,n_vertices;

	//ler verticais->horizontais->numero de vertices
	if (getline(stream_leitura, line)) {
		div_verticais = stoi(line);
	}
	if (getline(stream_leitura, line)) {
		div_horizontais = stoi(line);
	}
	if (getline(stream_leitura, line)) {
		n_vertices = stoi(line);
	}

	//alocar array de vertices e inicializar cone com divisoes
	cone_temp.conjunto_vertices = (vertice*)malloc(n_vertices*sizeof(vertice));
	cone_temp.n_vertices = n_vertices;
	cone_temp.div_verticais = div_verticais;
	cone_temp.div_horizontais = div_horizontais;

	//ler vertices

	while (n_vertices > 0) {
		if (!getline(stream_leitura, line)) break;
		std::stringstream ss(line);
		getline(ss, xString, ',');
		getline(ss, yString, ',');
		getline(ss, zString, ',');

		vertice_x = std::stof(xString);
		vertice_y = std::stof(yString);
		vertice_z = std::stof(zString);

		vertice v;
		v.x = vertice_x;
		v.y = vertice_y;
		v.z = vertice_z;
		cone_temp.conjunto_vertices[i] = v;
		/*stream_escrita << cone_temp.div_verticais << ","<<cone_temp.div_horizontais <<","<< cone_temp.n_vertices << std::endl;
		stream_escrita << cone_temp.conjunto_vertices[i].x << "," << cone_temp.conjunto_vertices[i].y << "," << cone_temp.conjunto_vertices[i].z << std::endl;*/
		i++;
		n_vertices--;
	}
	


}

void ler_Esfera(std::string ficheiro) {
	std::ifstream stream_leitura;
	stream_leitura.open(ficheiro);
	/*std::ofstream stream_escrita;
	stream_escrita.open("debugcone.txt");*/


	std::string xString, yString, zString, line;
	float vertice_x, vertice_y, vertice_z;
	int i = 0;
	int div_verticais, div_horizontais, n_vertices;

	//ler verticais->horizontais->numero de vertices
	if (getline(stream_leitura, line)) {
		div_verticais = stoi(line);
	}
	if (getline(stream_leitura, line)) {
		div_horizontais = stoi(line);
	}
	if (getline(stream_leitura, line)) {
		n_vertices = stoi(line);
	}

	//alocar array de vertices e inicializar cone com divisoes
	esfera_temp.conjunto_vertices = (vertice*)malloc(n_vertices * sizeof(vertice));
	esfera_temp.n_vertices = n_vertices;
	esfera_temp.div_verticais = div_verticais;
	esfera_temp.div_horizontais = div_horizontais;

	//ler vertices

	while (n_vertices > 0) {
		if (!getline(stream_leitura, line)) break;
		std::stringstream ss(line);
		getline(ss, xString, ',');
		getline(ss, yString, ',');
		getline(ss, zString, ',');

		vertice_x = std::stof(xString);
		vertice_y = std::stof(yString);
		vertice_z = std::stof(zString);

		vertice v;
		v.x = vertice_x;
		v.y = vertice_y;
		v.z = vertice_z;
		esfera_temp.conjunto_vertices[i] = v;
		/*stream_escrita << cone_temp.div_verticais << ","<<cone_temp.div_horizontais <<","<< cone_temp.n_vertices << std::endl;
		stream_escrita << cone_temp.conjunto_vertices[i].x << "," << cone_temp.conjunto_vertices[i].y << "," << cone_temp.conjunto_vertices[i].z << std::endl;*/
		i++;
		n_vertices--;
	}



}

void desenhar_Plano() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	glVertex3f(plano_temp.arrayVertice[0].x, plano_temp.arrayVertice[0].y, plano_temp.arrayVertice[0].z);
	glVertex3f(plano_temp.arrayVertice[1].x, plano_temp.arrayVertice[1].y, plano_temp.arrayVertice[1].z);
	glVertex3f(plano_temp.arrayVertice[2].x, plano_temp.arrayVertice[2].y, plano_temp.arrayVertice[2].z);

	glVertex3f(plano_temp.arrayVertice[0].x, plano_temp.arrayVertice[0].y, plano_temp.arrayVertice[0].z);
	glVertex3f(plano_temp.arrayVertice[2].x, plano_temp.arrayVertice[2].y, plano_temp.arrayVertice[2].z);
	glVertex3f(plano_temp.arrayVertice[3].x, plano_temp.arrayVertice[3].y, plano_temp.arrayVertice[3].z);
	glEnd();
}

void desenhar_cone(std::string modo_desenho) {
	/*std::ofstream stream_escrita;
	stream_escrita.open("debugconedesenho.txt");*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (modo_desenho.compare("pontos")==0) {
		/*stream_escrita << "entrou" << std::endl;*/
		
		for (int i = 0; i < cone_temp.n_vertices; i++) {
			glBegin(GL_POINTS);
			
			glVertex3f(cone_temp.conjunto_vertices[i].x, cone_temp.conjunto_vertices[i].y, cone_temp.conjunto_vertices[i].z);
			glEnd();


		}
	
	}


	if (modo_desenho.compare("triangulos")==0) {
		int pivot=0;
		int i , vertice_superior, vertice_inicial, vertice_superior_inicial;
		//desenho da base
		for (int i = 0; i < cone_temp.div_verticais-1; i++) {
			glBegin(GL_TRIANGLES);
			glVertex3f(0, 0, 0);
			glVertex3f(cone_temp.conjunto_vertices[i].x, cone_temp.conjunto_vertices[i].y, cone_temp.conjunto_vertices[i].z);
			glVertex3f(cone_temp.conjunto_vertices[i+1].x, cone_temp.conjunto_vertices[i+1].y, cone_temp.conjunto_vertices[i+1].z);
			glEnd();
		}
		//ultimo triangulo
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, 0);
		glVertex3f(cone_temp.conjunto_vertices[cone_temp.div_verticais - 1].x, cone_temp.conjunto_vertices[cone_temp.div_verticais - 1].y, cone_temp.conjunto_vertices[cone_temp.div_verticais - 1].z);
		glVertex3f(cone_temp.conjunto_vertices[0].x, cone_temp.conjunto_vertices[0].y, cone_temp.conjunto_vertices[0].z);
		
		glEnd();
		//desenho do cone ate a ultima divisao
		for (int j = 0; j < cone_temp.div_horizontais-1; j++) {
			i = 0;
			pivot = j * cone_temp.div_verticais;
			vertice_inicial = pivot;
			vertice_superior_inicial = pivot + cone_temp.div_verticais;
			while (i < cone_temp.div_verticais - 1) {
				vertice_superior = vertice_inicial + cone_temp.div_verticais;
				//stream_escrita << i << "," << j << "vertice_inicial" << vertice_inicial << "vertice superior" << vertice_superior << "vertice superior inicial" << vertice_superior_inicial << std::endl;
				glBegin(GL_QUADS);
				glVertex3f(cone_temp.conjunto_vertices[vertice_inicial].x, cone_temp.conjunto_vertices[vertice_inicial].y, cone_temp.conjunto_vertices[vertice_inicial].z);
				glVertex3f(cone_temp.conjunto_vertices[vertice_inicial + 1].x, cone_temp.conjunto_vertices[vertice_inicial + 1].y, cone_temp.conjunto_vertices[vertice_inicial + 1].z);
				glVertex3f(cone_temp.conjunto_vertices[vertice_superior + 1].x, cone_temp.conjunto_vertices[vertice_superior + 1].y, cone_temp.conjunto_vertices[vertice_superior + 1].z);
				glVertex3f(cone_temp.conjunto_vertices[vertice_superior].x, cone_temp.conjunto_vertices[vertice_superior].y, cone_temp.conjunto_vertices[vertice_superior].z);
				glEnd();
				i++;
				vertice_inicial++;
			}

			//desenhar o ultimo quad, é especial porque é preciso do vertice inicial do ciclo e o seu superior
			vertice_superior++;
			glBegin(GL_QUADS);
			glVertex3f(cone_temp.conjunto_vertices[vertice_inicial].x, cone_temp.conjunto_vertices[vertice_inicial].y, cone_temp.conjunto_vertices[vertice_inicial].z);
			glVertex3f(cone_temp.conjunto_vertices[pivot].x, cone_temp.conjunto_vertices[pivot].y, cone_temp.conjunto_vertices[pivot].z);
			glVertex3f(cone_temp.conjunto_vertices[vertice_superior_inicial].x, cone_temp.conjunto_vertices[vertice_superior_inicial].y, cone_temp.conjunto_vertices[vertice_superior_inicial].z);
			glVertex3f(cone_temp.conjunto_vertices[vertice_superior].x, cone_temp.conjunto_vertices[vertice_superior].y, cone_temp.conjunto_vertices[vertice_superior].z);
			glEnd();
		

		}
		/*stream_escrita << pivot << std::endl;*/
		pivot = vertice_superior_inicial;
		/*stream_escrita << pivot << std::endl;
		stream_escrita << cone_temp.n_vertices << std::endl;*/
		vertice_inicial = pivot;
		//desenho do topo
		for (int i = 0; i < cone_temp.div_verticais-1; i++) {
			
			glBegin(GL_TRIANGLES);
			glVertex3f(cone_temp.conjunto_vertices[vertice_inicial].x, cone_temp.conjunto_vertices[vertice_inicial].y, cone_temp.conjunto_vertices[vertice_inicial].z);
			glVertex3f(cone_temp.conjunto_vertices[vertice_inicial+1].x, cone_temp.conjunto_vertices[vertice_inicial+1].y, cone_temp.conjunto_vertices[vertice_inicial+1].z);
			glVertex3f(cone_temp.conjunto_vertices[cone_temp.n_vertices-1].x, cone_temp.conjunto_vertices[cone_temp.n_vertices - 1].y, cone_temp.conjunto_vertices[cone_temp.n_vertices - 1].z);
			glEnd();
			vertice_inicial++;
		}
		/*stream_escrita << vertice_inicial << std::endl;*/
		glBegin(GL_TRIANGLES);
		glVertex3f(cone_temp.conjunto_vertices[vertice_inicial].x, cone_temp.conjunto_vertices[vertice_inicial].y, cone_temp.conjunto_vertices[vertice_inicial].z);
		glVertex3f(cone_temp.conjunto_vertices[pivot].x, cone_temp.conjunto_vertices[pivot].y, cone_temp.conjunto_vertices[pivot].z);
		glVertex3f(cone_temp.conjunto_vertices[cone_temp.n_vertices - 1].x, cone_temp.conjunto_vertices[cone_temp.n_vertices - 1].y, cone_temp.conjunto_vertices[cone_temp.n_vertices - 1].z);
		glEnd();
	


	}
}

void desenhar_Esfera() {
	
	
	//glColor3b(1, 10, 80);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (int i = 1; i < esfera_temp.div_verticais+1; i++) {
		glVertex3f(esfera_temp.conjunto_vertices[0].x, esfera_temp.conjunto_vertices[0].y, esfera_temp.conjunto_vertices[0].z);
		glVertex3f(esfera_temp.conjunto_vertices[i].x, esfera_temp.conjunto_vertices[i].y, esfera_temp.conjunto_vertices[i].z);
		glVertex3f(esfera_temp.conjunto_vertices[i+1].x, esfera_temp.conjunto_vertices[i+1].y, esfera_temp.conjunto_vertices[i+1].z);

		
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	for (int i = 1; i < esfera_temp.n_vertices-esfera_temp.div_verticais-1; i++) {
		//glColor3b(10+i/400, 10 + i / 400, 10 + i / 400);
		glVertex3f(esfera_temp.conjunto_vertices[i].x, esfera_temp.conjunto_vertices[i].y, esfera_temp.conjunto_vertices[i].z);
		glVertex3f(esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais+1].x, esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais + 1].y, esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais + 1].z);
		glVertex3f(esfera_temp.conjunto_vertices[i + 1].x, esfera_temp.conjunto_vertices[i + 1].y, esfera_temp.conjunto_vertices[i + 1].z);
		glVertex3f(esfera_temp.conjunto_vertices[i + 1].x, esfera_temp.conjunto_vertices[i + 1].y, esfera_temp.conjunto_vertices[i + 1].z);
		glVertex3f(esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais + 2].x, esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais + 2].y, esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais + 2].z);
		glVertex3f(esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais + 1].x, esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais + 1].y, esfera_temp.conjunto_vertices[i + esfera_temp.div_verticais + 1].z);


	}
	glEnd();

}


void desenhar_Caixa() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	//baixo
	glVertex3f(caixa_temp.conjunto_vertices[0].x, caixa_temp.conjunto_vertices[0].y, caixa_temp.conjunto_vertices[0].z);
	glVertex3f(caixa_temp.conjunto_vertices[1].x, caixa_temp.conjunto_vertices[1].y, caixa_temp.conjunto_vertices[1].z);
	glVertex3f(caixa_temp.conjunto_vertices[2].x, caixa_temp.conjunto_vertices[2].y, caixa_temp.conjunto_vertices[2].z);
	glVertex3f(caixa_temp.conjunto_vertices[3].x, caixa_temp.conjunto_vertices[3].y, caixa_temp.conjunto_vertices[3].z);
	//cima
	glVertex3f(caixa_temp.conjunto_vertices[4].x, caixa_temp.conjunto_vertices[4].y, caixa_temp.conjunto_vertices[4].z);
	glVertex3f(caixa_temp.conjunto_vertices[5].x, caixa_temp.conjunto_vertices[5].y, caixa_temp.conjunto_vertices[5].z);
	glVertex3f(caixa_temp.conjunto_vertices[6].x, caixa_temp.conjunto_vertices[6].y, caixa_temp.conjunto_vertices[6].z);
	glVertex3f(caixa_temp.conjunto_vertices[7].x, caixa_temp.conjunto_vertices[7].y, caixa_temp.conjunto_vertices[7].z);

	glVertex3f(caixa_temp.conjunto_vertices[1].x, caixa_temp.conjunto_vertices[1].y, caixa_temp.conjunto_vertices[1].z);
	glVertex3f(caixa_temp.conjunto_vertices[5].x, caixa_temp.conjunto_vertices[5].y, caixa_temp.conjunto_vertices[5].z);
	glVertex3f(caixa_temp.conjunto_vertices[6].x, caixa_temp.conjunto_vertices[6].y, caixa_temp.conjunto_vertices[6].z);
	glVertex3f(caixa_temp.conjunto_vertices[2].x, caixa_temp.conjunto_vertices[2].y, caixa_temp.conjunto_vertices[2].z);
	


	glVertex3f(caixa_temp.conjunto_vertices[0].x, caixa_temp.conjunto_vertices[0].y, caixa_temp.conjunto_vertices[0].z);
	glVertex3f(caixa_temp.conjunto_vertices[4].x, caixa_temp.conjunto_vertices[4].y, caixa_temp.conjunto_vertices[4].z);
	glVertex3f(caixa_temp.conjunto_vertices[7].x, caixa_temp.conjunto_vertices[7].y, caixa_temp.conjunto_vertices[7].z);
	glVertex3f(caixa_temp.conjunto_vertices[3].x, caixa_temp.conjunto_vertices[3].y, caixa_temp.conjunto_vertices[3].z);
	

	glVertex3f(caixa_temp.conjunto_vertices[0].x, caixa_temp.conjunto_vertices[0].y, caixa_temp.conjunto_vertices[0].z);
	glVertex3f(caixa_temp.conjunto_vertices[1].x, caixa_temp.conjunto_vertices[1].y, caixa_temp.conjunto_vertices[1].z);
	glVertex3f(caixa_temp.conjunto_vertices[5].x, caixa_temp.conjunto_vertices[5].y, caixa_temp.conjunto_vertices[5].z);
	glVertex3f(caixa_temp.conjunto_vertices[4].x, caixa_temp.conjunto_vertices[4].y, caixa_temp.conjunto_vertices[4].z);


	glVertex3f(caixa_temp.conjunto_vertices[2].x, caixa_temp.conjunto_vertices[2].y, caixa_temp.conjunto_vertices[2].z);
	glVertex3f(caixa_temp.conjunto_vertices[6].x, caixa_temp.conjunto_vertices[6].y, caixa_temp.conjunto_vertices[6].z);
	glVertex3f(caixa_temp.conjunto_vertices[7].x, caixa_temp.conjunto_vertices[7].y, caixa_temp.conjunto_vertices[7].z);
	glVertex3f(caixa_temp.conjunto_vertices[3].x, caixa_temp.conjunto_vertices[3].y, caixa_temp.conjunto_vertices[3].z);
	

	
	glEnd();
	

}


void processNormalKeys(unsigned char c, int xx, int yy) {

	// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.8f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.8f; break;
	}
	spherical2Cartesian();
	//glutPostRedisplay();

}

void ler_xml(std::string ficheiro) {
	std::ofstream stream_escrita;
	stream_escrita.open("debugxml.txt");
	XMLDocument cena_xml;
	const char *c = ficheiro.c_str();
	cena_xml.LoadFile(c);
	stream_escrita << c << std::endl;
	XMLNode * cena_root = cena_xml.FirstChild();
	stream_escrita << cena_root<< std::endl;

	XMLElement *plano = cena_root->FirstChildElement("modelo");
	XMLElement *caixa = plano->NextSiblingElement();
	XMLElement *cone = caixa->NextSiblingElement();
	XMLElement *esfera = cone->NextSiblingElement();
	
	
	const char *nomeplano = plano->GetText();
	const char *nomecaixa = caixa->GetText();
	const char *nomecone = cone->GetText();
	const char *nomeesfera = esfera->GetText();

	stream_escrita << nomecaixa << nomeplano << nomecone << nomeesfera << std::endl;
	ler_Plano(nomeplano);
	ler_Caixa(nomecaixa);
	ler_Cone(nomecone);
	ler_Esfera(nomeesfera);
	stream_escrita << caixa_temp.n_vertices << std::endl;

}

int main(int argc, char **argv) {

// put GLUT init here
	//ler_Cone("cone50.3D");
	//ler_Caixa("caixa2.3D");
	//ler_Plano("plano.3D");
	//ler_Esfera("esfera200.3D");
	ler_xml("cena.xml");
	
	
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG_TP_FASE_1");



// put callback registration here
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glColor3f(0.0, 0.0, 0.0);   //this one
	glPointSize(1);             //and this one

// enter GLUT's main loop
	glutMainLoop();
	
	return 1;
}



