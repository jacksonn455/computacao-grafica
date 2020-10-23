#include "Scene1.h"


CScene1::CScene1()
{
	pCamera = NULL;
	pTexto = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 20.0f, 45.0f);

	// Cria gerenciador de mapeamento de texturas
	pTexture = new CTexture();
	pTexture->CreateTextureLinear(0, "../Scene1/floorSnow.jpg");
	pTexture->CreateTextureLinear(1, "../Scene1/frozen.jpg");
	pTexture->CreateTextureLinear(2, "../Scene1/floorSnow.jpg");
	pTexture->CreateTextureMipMap(3, "../Scene1/neve.bmp");
	pTexture->CreateTextureMipMap(4, "../Scene1/door.jpg");
	pTexture->CreateTextureMipMap(5, "../Scene1/sangue1.png");


	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;
	fPosX = 0.0f;
	fPosY = 5.0f;
	fPosZ = 10.0f;
	fMovementFactor = 0.1f;

	fRotX = 0.0f;
	fRotY = 1.0f;
	fRotZ = 0.0f;
	fAngle = 0.0f;

	fDistance = 0.0f;

	// Define os componentes para a fonte de luz (Point Light)
	// A fonte de luz emite a cor branca com intensidade máxima para toda a cena
	LightAmbient[0] = 1.0f;
	LightAmbient[1] = 1.0f;
	LightAmbient[2] = 1.0f;
	LightAmbient[3] = 1.0f;

	LightDiffuse[0] = 1.0f;
	LightDiffuse[1] = 1.0f;
	LightDiffuse[2] = 1.0f;
	LightDiffuse[3] = 1.0f;

	LightSpecular[0] = 1.0f;
	LightSpecular[1] = 1.0f;
	LightSpecular[2] = 1.0f;
	LightSpecular[3] = 1.0f;

	LightPosition[0] = 0.3f;
	LightPosition[1] = 0.3f;
	LightPosition[2] = 0.3f;
	LightPosition[3] = 0.29f;


	// Cria um display list
	DrawObjectWithDisplayList();

	DrawGengon();
}


CScene1::~CScene1(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pTexture)
	{
		delete pTexture;
		pTexture = NULL;
	}
}




int CScene1::DrawGLScene(void)	// Função que desenha a cena
{
	// Cilindro & Cone
	GLUquadricObj* quadObj = gluNewQuadric();

	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter

	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();

	// Desenha grid 
	//Draw3DSGrid(20.0f, 20.0f);

	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Atualiza a posição da lâmpada baseado nas teclas direcionais do teclado
	LightPosition[0] = fPosX;
	LightPosition[1] = fPosY;
	LightPosition[2] = fPosZ;

	// Desenha a lâmpada
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(LightPosition[0], LightPosition[1], LightPosition[2]);
	glutSolidSphere(0.2f, 5, 5);
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Habilita iluminação

	// Definindo as propriedades da fonte de luz	
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	glEnable(GL_LIGHT0); // Liga a lâmpada 0

	fAngle += 0.5f;
	if (fAngle >= 360.0f)
		fAngle = 0.0f;
	// Define o material para reflexão do objeto
	// O material é aplicado na superfície do objeto, e define quanto
	// o objeto irá absorver ou refletir da luz
	MatAmbient[0] = 0.0f;	MatAmbient[1] = 0.0f;	MatAmbient[2] = 0.0f;	MatAmbient[3] = 50.0f;
	MatDiffuse[0] = 50.0f;	MatDiffuse[1] = 50.0f;	MatDiffuse[2] = 50.0f;	MatDiffuse[3] = 50.0f;
	MatSpecular[0] = 0.0f;	MatSpecular[1] = 0.0f;	MatSpecular[2] = 0.0f;	MatSpecular[3] = 50.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, 128);


	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0); // Define a cor atual

	// Desenha a neve 
	glPushMatrix();
	pTexture->ApplyTexture(3); // Define a textura atual
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2d(2.0f, 0.0f); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2d(2.0f, 2.0f); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2d(0.0f, 2.0f); glVertex3f(-100.0f, 0.0f, -100.0f);
	glEnd();
	glPopMatrix();

	//BONECO
	DrawSnowMan(0.0f, 0.0, 0.0, 10);

	// DESENHA O FOSSO
	DrawCube(0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		75.0f, 0.0f, 75.0f,
		1);

	// DESENHA A BASE
	DrawCube(0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		50.0f, 1.5f, 50.0f,
		0);


	// DESENHA A TETO
	DrawCube(0.0f, 15.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		40.0f, 10.0f, 35.0f,
		0);

	// DESENHA A PONTE
	DrawCube(0.0f, 1.0f, 7.8f,
		0.0f, 0.0f, 0.0f, 0.0f,
		10.0f, 0.5f, 60.0f,
		0);

	// DESENHA AS PAREDES
	DrawCube(-20.0f, 10.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		2.0, 20.0, 30.0,
		2);
	DrawCube(20.0f, 10.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		2.0, 20.0, 30.0,
		2);
	DrawCube(0.0f, 10.5f, -20.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		30.0, 20.0, 2.0,
		2);
	DrawCube(0.0f, 10.5f, 20.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		30.0, 20.0, 2.0,
		2);

	// MURALHA
	DrawCube(-79.0f, 20.3f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		2.0, 40.0, 157.0,
		2);
	DrawCube(79.0f, 20.3f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		2.0, 40.0, 157.0,
		2);
	DrawCube(0.0f, 20.3f, -79.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		157.0, 40.0, 2.0,
		2);
	DrawCube(50.0f, 20.3f, 79.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		60, 40.0, 2.0,
		2);
	DrawCube(-50.0f, 20.3f, 79.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		60, 40.0, 2.0,
		2);

	// TORRE
	glPushMatrix();
	glTranslatef(20, -8.5f, 20);
	glRotatef(-90, 1, 0, 0);
	DrawCylinder(1, 16, 22, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-20, -8.5f, 20);
	glRotatef(-90, 1, 0, 0);
	DrawCylinder(1, 16, 22, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(20, -8.5f, -20);
	glRotatef(-90, 1, 0, 0);
	DrawCylinder(1, 16, 22, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-20, -8.5f, -20);
	glRotatef(-90, 1, 0, 0);
	DrawCylinder(1, 16, 22, 5);
	glPopMatrix();

	//PORTÃO
	DrawCube(0.0f, 20.3f, 79.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		40.0, 40.0, 1,
		4);

	// PORTA
	DrawCube(0.0f, 6.5f, 21.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		8.0, 12.0, 0.5,
		4);

	// ARVORE
	glPushMatrix();
	glTranslatef(-40, -4.0f, -10);
	glRotatef(-90, 1, 0, 0);
	DrawCylinder(1, 16, 8, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-40, 8, -10);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0, 128, 0);
	auxSolidCone(4, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(75, 54, 33);
	glTranslatef(45, -4.0f, -10);
	glRotatef(-90, 1, 0, 0);
	DrawCylinder(1, 16, 8, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(45, 8, -10);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0, 128, 0);
	auxSolidCone(4, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(75, 54, 33);
	glTranslatef(45, -4.0f, 25);
	glRotatef(-90, 1, 0, 0);
	DrawCylinder(1, 16, 8, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(45, 8, 25);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0, 128, 0);
	auxSolidCone(4, 10);
	glPopMatrix();

	// CHAPEU TORRE
	glPushMatrix();
	glTranslatef(20, 24.5, 20);
	glRotatef(-90, 1, 0, 0);
	glColor3ub(10, 10, 10);
	auxSolidCone(6, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-20, 24.5, 20);
	glRotatef(-90, 1, 0, 0);
	glColor3ub(10, 10, 10);
	auxSolidCone(6, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(20, 24.5, -20);
	glRotatef(-90, 1, 0, 0);
	glColor3ub(10, 10, 10);
	auxSolidCone(6, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-20, 24.5, -20);
	glRotatef(-90, 1, 0, 0);
	glColor3ub(10, 10, 10);
	auxSolidCone(6, 10);
	glPopMatrix();

	//telhado castelo
	glPushMatrix();
	glTranslatef(0, 20.5, 0);
	glRotatef(-90, 1, 0, 0);
	glColor3ub(10, 10, 10);
	auxSolidCone(21, 23);
	glPopMatrix();

	//BONECO
	DrawSnowMan(0.0f, 0.0, 0.0, 10);

	glDisable(GL_LIGHT0);	// Desliga a lâmpada
	glDisable(GL_LIGHTING);	// Desabilita iluminação

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime() / 1000));

	glRasterPos2f(10.0f, 100.0f);
	pTexto->glPrint("fDistance: %f", fDistance);

	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene1::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene1::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}


	if (GetKeyState(VK_UP) & 0x80)
	{
		fPosZ -= fMovementFactor;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		fPosZ += fMovementFactor;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		fPosX -= fMovementFactor;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		fPosX += fMovementFactor;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		fPosY += fMovementFactor;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		fPosY -= fMovementFactor;
	}
}

void CScene1::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
	{
		pTimer->Init();
	}
	break;

	case VK_RETURN:

		break;

	case 'X':
	{
		fRotX = 1.0f;
		fRotY = 0.0f;
		fRotZ = 0.0f;
	}
	break;

	case 'Y':
	{
		fRotX = 0.0f;
		fRotY = 1.0f;
		fRotZ = 0.0f;
	}
	break;

	case 'Z':
	{
		fRotX = 0.0f;
		fRotY = 0.0f;
		fRotZ = 1.0f;
	}
	break;
	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene1::Draw3DSGrid(float width, float length)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void CScene1::DrawCube(float pX, float pY, float pZ,
	float rX, float rY, float rZ, float angle,
	float sX, float sY, float sZ,
	int texID)
{

	// Seta a textura ativa
	if (texID >= 0)
		pTexture->ApplyTexture(texID);

	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	glRotatef(angle, rX, rY, rZ);
	glScalef(sX, sY, sZ);

	glBegin(GL_QUADS);
	// face frente
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);

	// face trás
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);

	// face direita
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);

	// face esquerda
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

	// face baixo
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);

	// face cima
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

	glEnd();

	glPopMatrix();


}

void CScene1::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}

//void CScene1::CreateSkyBox(float x, float y, float z, float width, float height, float length)
//{
//	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//	glPushMatrix();
//
//	// Centraliza o Skybox em torno da posição especificada(x, y, z)
//	x = x - width / 2;
//	y = y - height / 2;
//	z = z - length / 2;
//
//
//	// Aplica a textura que representa a parte da frente do skybox (BACK map)
//	pTextures->ApplyTexture(0);
//
//	// Desenha face BACK do cubo do skybox
//	glBegin(GL_QUADS);
//	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
//	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
//	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
//	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
//	glEnd();
//
//
//	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
//	pTextures->ApplyTexture(1);
//
//	// Desenha face FRONT do cubo do skybox
//	glBegin(GL_QUADS);
//	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
//	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
//	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
//	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
//	glEnd();
//
//
//	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
//	pTextures->ApplyTexture(2);
//
//	// Desenha face BOTTOM do cubo do skybox
//	glBegin(GL_QUADS);
//	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
//	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
//	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
//	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
//	glEnd();
//
//
//	// Aplica a textura que representa a parte da frente do skybox (UP map)
//	pTextures->ApplyTexture(3);
//
//	// Desenha face TOP do cubo do skybox
//	glBegin(GL_QUADS);
//	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
//	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
//	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
//	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
//	glEnd();
//
//
//	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
//	pTextures->ApplyTexture(4);
//
//	// Desenha face LEFT do cubo do skybox
//	glBegin(GL_QUADS);
//	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
//	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
//	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
//	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
//	glEnd();
//
//
//	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
//	pTextures->ApplyTexture(5);
//
//	// Desenha face RIGHT do cubo do skybox
//	glBegin(GL_QUADS);
//	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
//	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
//	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
//	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
//	glEnd();
//
//	glPopMatrix();
//}

void CScene1::DrawGengon()
{
	// Cria um novo Display List
	this->genGonDisplayListID = glGenLists(1); // Cria um ID para o DisplayList	
	// Inicia a definição do objeto em DisplayList
	glNewList(this->genGonDisplayListID, GL_COMPILE);

	for (int i = 0; i < 180; i += 9)
	{
		glBegin(GL_TRIANGLES);
		Vec3 normal; // Calcula o vetor Normal para o quadrado
		normal = CalculateTriangleNormalVector(
			Vec3(Gengon[i], Gengon[i + 1], Gengon[i + 2]),
			Vec3(Gengon[i + 3], Gengon[i + 4], Gengon[i + 5]),
			Vec3(Gengon[i + 6], Gengon[i + 7], Gengon[i + 8]));
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(Gengon[i], Gengon[i + 1], Gengon[i + 2]);
		glVertex3f(Gengon[i + 3], Gengon[i + 4], Gengon[i + 5]);
		glVertex3f(Gengon[i + 6], Gengon[i + 7], Gengon[i + 8]);
		glEnd();
	}

	glEndList(); // Encerra a definição do objeto em DisplayList
}

void CScene1::DrawObjectWithDisplayList()
{
	// Cria um novo Display List
	this->objectDisplayListID = glGenLists(1); // Cria um ID para o DisplayList	
	// Inicia a definição do objeto em DisplayList
	glNewList(this->objectDisplayListID, GL_COMPILE);

	glBegin(GL_QUADS); // Cria um quadrado
	Vec3 normal; // Calcula o vetor Normal para o quadrado
	normal = CalculateQuadNormalVector(
		Vec3(-5.0f, 0.0f, 0.0f),
		Vec3(5.0f, 0.0f, 0.0f),
		Vec3(5.0f, 10.0f, 0.0f),
		Vec3(-5.0f, 10.0f, 0.0f));
	glNormal3f(normal.x, normal.y, normal.z); // Aplica a Normal nos vértices
	glVertex3f(-5.0f, 0.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);
	glVertex3f(5.0f, 10.0f, 0.0f);
	glVertex3f(-5.0f, 10.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	normal = CalculateTriangleNormalVector(
		Vec3(-5.0f, 10.0f, 0.0f),
		Vec3(5.0f, 10.0f, 0.0f),
		Vec3(0.0f, 20.0f, 0.0f));
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(-5.0f, 10.0f, 0.0f);
	glVertex3f(5.0f, 10.0f, 0.0f);
	glVertex3f(0.0f, 20.0f, 0.0f);
	glEnd();

	glEndList(); // Encerra a definição do objeto em DisplayList
}

void CScene1::DrawCylinder(int numMajor, int numMinor, float height, float radius)
{
	double majorStep = height / numMajor;
	double minorStep = 2.0 * 3.14159265 / numMinor;
	int i, j;

	for (i = 0; i < numMajor; ++i)
	{
		GLfloat z0 = 0.5 * height - i + majorStep;
		GLfloat z1 = z0 - majorStep;

		glBegin(GL_TRIANGLE_STRIP);

		for (j = 0; j <= numMinor; ++j)
		{
			double a = j * minorStep;
			GLfloat x = radius * cos(a);
			GLfloat y = radius * sin(a);
			glNormal3f(x / radius, y / radius, 0.0);
			glTexCoord2f(j / (GLfloat)numMinor, i / (GLfloat)numMajor);
			glVertex3f(x, y, z0);

			glNormal3f(x / radius, y / radius, 0.0);
			glTexCoord2f(j / (GLfloat)numMinor, (i + 1) / (GLfloat)numMajor);
			glVertex3f(x, y, z1);
		}

		glEnd();
	}


}

Vec3 CScene1::CalculateQuadNormalVector(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4)
{
	/*
		   v4      v3
			+------+
			|      |
			|      |
			+------+
		   v1      v2
	*/

	Vec3 U, V;
	U.x = v2.x - v1.x;
	U.y = v2.y - v1.y;
	U.z = v2.z - v1.z;

	V.x = v4.x - v1.x;
	V.y = v4.y - v1.y;
	V.z = v4.z - v1.z;


	// Calcula o vetor normal -> CrossProduct((v2-v1), (v4-v1))
	Vec3 normal;
	normal.x = U.y * V.z - U.z * V.y;
	normal.y = U.z * V.x - U.x * V.z;
	normal.z = U.x * V.y - U.y * V.x;

	// Normaliza o vetor
	double magnitude = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
	Vec3 NormalizedVector;
	NormalizedVector.x = (normal.x / magnitude);
	NormalizedVector.y = (normal.y / magnitude);
	NormalizedVector.z = (normal.z / magnitude);

	return NormalizedVector;
}

Vec3 CScene1::CalculateTriangleNormalVector(Vec3 v1, Vec3 v2, Vec3 v3)
{
	/*
			   v3
			   /\
			  /  \
			 /    \
			/      \
			+------+
		   v1      v2
	*/

	Vec3 U, V;
	U.x = v2.x - v1.x;
	U.y = v2.y - v1.y;
	U.z = v2.z - v1.z;

	V.x = v3.x - v1.x;
	V.y = v3.y - v1.y;
	V.z = v3.z - v1.z;


	// Calcula o vetor normal -> CrossProduct((v2-v1), (v3-v1))
	Vec3 normal;
	normal.x = U.y * V.z - U.z * V.y;
	normal.y = U.z * V.x - U.x * V.z;
	normal.z = U.x * V.y - U.y * V.x;

	// Normaliza o vetor
	double magnitude = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
	Vec3 NormalizedVector;
	NormalizedVector.x = (normal.x / magnitude);
	NormalizedVector.y = (normal.y / magnitude);
	NormalizedVector.z = (normal.z / magnitude);

	return NormalizedVector;
}

//BONECO
void CScene1::DrawSnowMan(float rot, float pX, float pY, float pZ)
{
	glPushMatrix();
	glTranslatef(-50, 5.0f, 30);
	glRotatef(360, 1, 0, 0);

	glColor3ub(255, 255, 255);
	glPushMatrix();

	//PERNAS
	glTranslatef(0.0f, 0.0f, 0.0f);
	auxSolidSphere(5.0);

	//TRONCO
	glTranslatef(0.0f, 6.0f, 0.0f);
	auxSolidSphere(3.0);

	//BOTOES DA CAMISA
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.9f);
	glTranslatef(0.0f, 1.0f, 3.0f);
	auxSolidSphere(0.5);

	glTranslatef(0.0f, -1.5f, 0.0f);
	auxSolidSphere(0.5);
	glPopMatrix();

	//CABEÇA
	glColor3ub(255, 255, 255);
	glTranslatef(0.0f, 4.0f, 0.0f);
	auxSolidSphere(2.0);

	//NARIZ
	glPushMatrix();
	glColor3ub(200, 128, 0);
	glTranslatef(0.0f, 0.0f, 2.0f);
	auxSolidCone(0.5, 2.0);

	//OLHOS
	glColor3ub(0, 0, 255);
	glTranslatef(1.0f, 0.5f, -0.5f);
	auxSolidSphere(0.3);
	glTranslatef(-2.0f, 0.5f, -0.0f);
	auxSolidSphere(0.3);
	glPopMatrix();

	//CHAPEU
	glColor3ub(0, 0, 255);
	glTranslatef(0.0f, 1.0f, 0.0f);
	auxSolidCylinder(2.5f, 0.8f);
	glTranslatef(0.0f, 2.0f, 0.0f);
	auxSolidCylinder(2.0f, 2.0f);

	glPopMatrix();
	glPopMatrix();
}