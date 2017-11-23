
//-----------------------------------������˵����----------------------------------------------
//  ��������:��GDIdemo10
//	 2013��3�� Create by ǳī
//  ������Windows��Ϣ����֮������Ϣ���� ʾ������
//------------------------------------------------------------------------------------------------

//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
#include <windows.h>
#include <tchar.h>//ʹ��swprintf_s���������ͷ�ļ�

//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")			//����PlaySound����������ļ�
#pragma  comment(lib,"Msimg32.lib")		//���ʹ��TransparentBlt��������Ŀ��ļ�

//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE		L"������������Ϩ�����Ϸ�������롿Windows��Ϣ����֮ ������Ϣ���� "	//Ϊ���ڱ��ⶨ��ĺ�

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
HDC				g_hdc=NULL,g_mdc=NULL,g_bufdc=NULL;      //ȫ���豸�������������ȫ���ڴ�DC���
HBITMAP		g_hSprite[4]={NULL},g_hBackGround=NULL;								//����λͼ����������ڴ洢���ŷ���ͼ���Լ�����洢����ͼ�ľ��
DWORD		g_tPre=0,g_tNow=0;					//����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
int					g_iNum=0,g_iX=0,g_iY=0;                //g_iNum������¼ͼ�ţ�g_iX��g_iY�ֱ��ʾ��ͼ�ĺ�������
int					g_iDirection=0;//g_iDirectionΪ�����ƶ�����������������0��1��2��3���������ϣ��£����ҷ����ϵ��ƶ�

//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );//���ڹ��̺���
BOOL						Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID							Game_Paint( HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL						Game_CleanUp(HWND hwnd );	//�ڴ˺����н�����Դ������

//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra		= 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra		= 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //ָ��������Ĺ������
	wndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow( L"ForTheDreamOfGameDevelop",WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow( hwnd, nShowCmd );    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init (hwnd)) 
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����
		return FALSE;
	}
	PlaySound(L"�ɽ����»�.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP); //ѭ�����ű������� 

	//��5����Ϣѭ������
	MSG msg = { 0 };				//���岢��ʼ��msg
	while( msg.message != WM_QUIT )		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage( &msg );			//�ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			g_tNow = GetTickCount();   //��ȡ��ǰϵͳʱ��
			if(g_tNow-g_tPre >= 50)        //���˴�ѭ���������ϴλ�ͼʱ�����0.05��ʱ�ٽ����ػ����
				Game_Paint(hwnd);
		}

	}

	//��6���������ע��
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //����׼��������ע��������
	return 0;  
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{

	switch( message )						//switch��俪ʼ
	{

	case WM_KEYDOWN:	     //���¼�����Ϣ
		//�жϰ������������
		switch (wParam) 
		{
		case VK_ESCAPE:           //���¡�Esc����
			DestroyWindow(hwnd);    // ���ٴ���, ������һ��WM_DESTROY��Ϣ
			PostQuitMessage( 0 );  //��������
			break;
		case VK_UP:				  //���¡�������
			//���ݰ������������ƶ�������ÿ�ΰ���һ�ΰ����ƶ�10����λ����������������ͼ�����X��Yֵ�������ж������Ƿ񳬳����������������������
			g_iY -= 10;
			g_iDirection = 0;
			if(g_iY < 0)
				g_iY = 0;
			break;
		case VK_DOWN:			  //���¡�������
			g_iY += 10;
			g_iDirection = 1;
			if(g_iY > WINDOW_HEIGHT-135)
				g_iY = WINDOW_HEIGHT-135;	
			break;
		case VK_LEFT:			  //���¡�������			
			g_iX -= 10;
			g_iDirection = 2;
			if(g_iX < 0)
				g_iX = 0;		
			break;
		case VK_RIGHT:			   //���¡�������
			g_iX += 10;
			g_iDirection = 3;
			if(g_iX > WINDOW_WIDTH-75)
				g_iX = WINDOW_WIDTH-75;
			break;
		}			
		break;								//������switch���

	case WM_DESTROY:					//���Ǵ���������Ϣ
		Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage( 0 );			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc( hwnd, message, wParam, lParam );		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

//-----------------------------------��Game_Init( )������--------------------------------------
//	��������ʼ������������һЩ�򵥵ĳ�ʼ��
//------------------------------------------------------------------------------------------------
BOOL Game_Init( HWND hwnd )
{
	HBITMAP bmp;

	g_hdc = GetDC(hwnd);  
	g_mdc = CreateCompatibleDC(g_hdc);  //����һ����hdc���ݵ��ڴ�DC
	g_bufdc = CreateCompatibleDC(g_hdc);//�ٴ���һ����hdc���ݵĻ���DC
	bmp = CreateCompatibleBitmap(g_hdc,WINDOW_WIDTH,WINDOW_HEIGHT);

	//�趨������ͼ��ʼλ�ú��ƶ�����
	g_iX = 150;
	g_iY = 350;
	g_iDirection = 3;
	g_iNum = 0;

	SelectObject(g_mdc,bmp);
	//���ظ����ܶ�ͼ������ͼ��������0,1,2,3������������ϣ��£������ƶ�
	g_hSprite[0] = (HBITMAP)LoadImage(NULL,L"go1.bmp",IMAGE_BITMAP,480,216,LR_LOADFROMFILE);
	g_hSprite[1] = (HBITMAP)LoadImage(NULL,L"go2.bmp",IMAGE_BITMAP,480,216,LR_LOADFROMFILE);
	g_hSprite[2] = (HBITMAP)LoadImage(NULL,L"go3.bmp",IMAGE_BITMAP,480,216,LR_LOADFROMFILE);
	g_hSprite[3] = (HBITMAP)LoadImage(NULL,L"go4.bmp",IMAGE_BITMAP,480,216,LR_LOADFROMFILE);
	g_hBackGround = (HBITMAP)LoadImage(NULL,L"bg.bmp",IMAGE_BITMAP,WINDOW_WIDTH,WINDOW_HEIGHT,LR_LOADFROMFILE);

	Game_Paint(hwnd);
	return TRUE;
}

//-----------------------------------��Game_Paint( )������--------------------------------------
//	���������ƺ������ڴ˺����н��л��Ʋ���
//--------------------------------------------------------------------------------------------------
VOID Game_Paint( HWND hwnd )
{
	//����mdc�����ϱ���ͼ
	SelectObject(g_bufdc,g_hBackGround);
	BitBlt(g_mdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_bufdc,0,0,SRCCOPY);

	//����Ŀǰ���ƶ�����ȡ����Ӧ����������߶�ͼ����ȷ����ȡ����ͼ�Ŀ����߶�
	SelectObject(g_bufdc,g_hSprite[g_iDirection]);
	BitBlt(g_mdc,g_iX,g_iY,60,108,g_bufdc,g_iNum*60,108,SRCAND);
	BitBlt(g_mdc,g_iX,g_iY,60,108,g_bufdc,g_iNum*60,0,SRCPAINT);
	//�����Ļ�����ʾ�ڴ�����
	BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_mdc,0,0,SRCCOPY);

	g_tPre = GetTickCount();     //��¼�˴λ�ͼʱ��
	g_iNum++;
	if(g_iNum == 8)
		g_iNum = 0;

}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp( HWND hwnd )
{
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	for (int i=0;i<4;i++)
	{
		DeleteObject(g_hSprite[i]);
	}
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd,g_hdc);
	return TRUE;
}