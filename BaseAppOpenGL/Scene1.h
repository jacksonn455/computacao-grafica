#pragma once
#include "SceneBaseClass.h"
#include "CShaderManager.h"
#include "CTexture.h"
#include "CTimer.h"
#include "CTexto.h"
#include "CCamera.h"
#include "CModel_3DS.h"
#include <vector>
#include "GL.H"
#include "GLU.H"
#include "glut.h"
#include "GLAux.h"
#include "math.h"


class Vec3
{
public:
	double x;
	double y;
	double z;

	Vec3()
	{

	}

	Vec3(double _x, double _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

class CScene1 : public CSceneBaseClass
{
public:
	CScene1();
	~CScene1(void);

	virtual void MouseMove(void);					// Tratamento de movimento do mouse
	virtual void KeyPressed(void);					// Tratamento de teclas pressionadas
	virtual void KeyDownPressed(WPARAM	wParam);	// Tratamento de teclas pressionadas
	virtual int DrawGLScene(void);					// Fun��o que desenha a cena

	void Draw3DSGrid(float width, float length);
	void DrawAxis();

	void DrawObjectWithDisplayList();
	Vec3 CalculateQuadNormalVector(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4);
	Vec3 CalculateTriangleNormalVector(Vec3 v1, Vec3 v2, Vec3 v3);
	void DrawGengon();

	void DrawCylinder(int numMajor, int numMinor, float height, float radius);

	void DrawCube(float pX = 0.0f, float pY = 0.0f, float pZ = 0.0f,
		float rX = 0.0f, float rY = 0.0f, float rZ = 0.0f, float angle = 0.0f,
		float sX = 1.0f, float sY = 1.0f, float sZ = 1.0f,
	    int texID = -1);

private:

	bool	keys[256];		// Array usado para rotinas do teclado
	bool	active;			// Window Active Flag Set To TRUE By Default
	bool	fullscreen;		// Exibir janela em modo fullscreem (TRUE) ou em janela (FALSE)	

	CCamera* pCamera;	// Gerencia c�mera OpenGL
	float fDeltaY;				// Rota��o da c�mera OpenGL no eixo Y
	float fDeltaX;				// Rota��o da c�mera OpenGL no eixo X

	CTexto* pTexto;			// Objeto que gerencia texto	
	CTimer* pTimer;			// Objeto que gerencia o timer
	CTexture* pTexture;		// Objeto que gerencia o mapeamento de texturas

	int		iFPS;			// FPS and FPS Counter
	int		iFrames;		// FPS and FPS Counter
	DWORD	ulLastFPS;		// FPS and FPS Counter
	char	szTitle[256];	// FPS and FPS Counter


	bool bIsWireframe;	// Modos Wireframe/Solid
	bool bIsCameraFPS;	// Ativa modo de camera First Person Shooter (true) ou Third Person Shooter (false)

	float fRenderPosY;
	float fTimerPosY;
	float fPosX;
	float fPosY;
	float fPosZ;
	float fMovementFactor;

	float fRotX;
	float fRotY;
	float fRotZ;
	float fAngle;

	float fDistance;

	// Define os componentes para a fonte de luz (Point Light)
	GLfloat LightAmbient[4];
	GLfloat LightDiffuse[4];
	GLfloat LightSpecular[4];
	GLfloat LightPosition[4];

	// Define o material para reflex�o do objeto
	// O material � aplicado na superf�cie do objeto, e define quanto
	// o objeto ir� absorver ou refletir da luz
	GLfloat MatAmbient[4];
	GLfloat MatDiffuse[4];
	GLfloat MatSpecular[4];

	// Cria um ID para um objeto DisplayList
	GLuint objectDisplayListID;
	GLuint genGonDisplayListID;

	GLfloat Gengon[180] = {
			-16.36,33.09,-4.27,
			4.68,33.09,-4.27,
			-9.86,33.09,-24.28,

			-16.36,33.09,-4.27,
			-9.86,33.09,-24.28,
			-33.37,33.09,-16.64,

			-16.36,33.09,-4.27,
			-33.37,33.09,-16.64,
			-33.37,33.09,8.09,

			-16.36,33.09,-4.27,
			-33.37,33.09,8.09,
			-9.86,33.09,15.73,

			-16.36,33.09,-4.27,
			-9.86,33.09,15.73,
			4.68,33.09,-4.27,

			4.68,33.09,-4.27,
			4.68,0.00,-4.27,
			-9.86,0.00,-24.28,

			4.68,33.09,-4.27,
			-9.86,0.00,-24.28,
			-9.86,33.09,-24.28,

			-9.86,33.09,-24.28,
			-9.86,0.00,-24.28,
			-33.37,0.00,-16.64,

			-9.86,33.09,-24.28,
			-33.37,0.00,-16.64,
			-33.37,33.09,-16.64,

			-33.37,33.09,-16.64,
			-33.37,0.00,-16.64,
			-33.37,0.00,8.09,

			-33.37,33.09,-16.64,
			-33.37,0.00,8.09,
			-33.37,33.09,8.09,

			-33.37,33.09,8.09,
			-33.37,0.00,8.09,
			-9.86,0.00,15.73,

			-33.37,33.09,8.09,
			-9.86,0.00,15.73,
			-9.86,33.09,15.73,

			-9.86,33.09,15.73,
			-9.86,0.00,15.73,
			4.68,0.00,-4.27,

			-9.86,33.09,15.73,
			4.68,0.00,-4.27,
			4.68,33.09,-4.27,

			4.68,0.00,-4.27,
			-16.36,0.00,-4.27,
			-9.86,0.00,-24.28,

			-9.86,0.00,-24.28,
			-16.36,0.00,-4.27,
			-33.37,0.00,-16.64,

			-33.37,0.00,-16.64,
			-16.36,0.00,-4.27,
			-33.37,0.00,8.09,

			-33.37,0.00,8.09,
			-16.36,0.00,-4.27,
			-9.86,0.00,15.73,

			-9.86,0.00,15.73,
			-16.36,0.00,-4.27,
			4.68,0.00,-4.27	};
};

