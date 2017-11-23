
//-----------------------------------������˵����----------------------------------------------
//  ��������:��GDIdemo14
//	 2013��3�� Create by ǳī
//  ��������ŭ��С��Ħ��������ģ�� ʾ������
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
#define WINDOW_TITLE		L"������������Ϩ�����Ϸ�������롿��ŭ��С��Ħ��������ģ��demo"	//Ϊ���ڱ��ⶨ��ĺ�


//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
HDC				g_hdc=NULL,g_mdc=NULL,g_bufdc=NULL;      //ȫ���豸���������ȫ���ڴ�DC���
HBITMAP		g_hAngrybird=NULL,g_hBackGround=NULL;  							//����λͼ����������ڴ洢ͼƬ�ز�
DWORD		g_tPre=0,g_tNow=0;					//����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
int		g_iX,g_iY,g_iXSpeed,g_iYSpeed; //g_iX,g_iY������ͼλ�ã�g_iXSpeed,g_iYSpeed��ʾX������Y�����ϵ��ٶ�
int		g_iYGravity=0,g_iXFriction=0,g_iYFriction=0;     //�������ٶ�g_iYGravity��x����Ħ����Ϊg_iXFriction��f����Ħ����Ϊg_iYFriction
RECT	  g_rect;				//����һ��RECT�ṹ�壬���ڴ����ڲ��������������


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
	PlaySound(L"NOIR2.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP); //ѭ�����ű������� 

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
			if(g_tNow-g_tPre >= 40)        //���˴�ѭ���������ϴλ�ͼʱ�����0.04��ʱ�ٽ����ػ����
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
	case WM_KEYDOWN:					//������Ϣ
		if(wParam==VK_ESCAPE)		//���¡�Esc����
			PostQuitMessage(0);
		break;

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
	bmp = CreateCompatibleBitmap(g_hdc,WINDOW_WIDTH,WINDOW_HEIGHT); //��һ���ʹ��ڼ��ݵĿյ�λͼ����

	SelectObject(g_mdc,bmp);//����λͼ����ŵ�mdc��

	g_hBackGround = (HBITMAP)LoadImage(NULL,L"bg.bmp",IMAGE_BITMAP,WINDOW_WIDTH,WINDOW_HEIGHT,LR_LOADFROMFILE);
	g_hAngrybird = (HBITMAP)LoadImage(NULL,L"angrybird.bmp",IMAGE_BITMAP,140,70,LR_LOADFROMFILE);

	GetClientRect(hwnd,&g_rect);		//ȡ���ڲ���������Ĵ�С

	//���ø����ʼֵ
	g_iX=0;  //��ʼ������g_iX=0
	g_iY=100;//��ʼ������g_iY=100
	g_iXSpeed=8;//��ʼˮƽ�����ٶ�g_iXSpeed=8
	g_iYSpeed=0; //��ʼ��ֱ�����ٶ�g_iYSpeed=0
	g_iYGravity=3;//�������ٶ���������Ϊ����ʾ���㣬����Ϊ3
	g_iXFriction=-1;  //ˮƽ����Ħ������Ϊ-1
	g_iYFriction=-4;  //��ֱ����Ħ������Ϊ-4

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

	//����С��ͼ
	SelectObject(g_bufdc,g_hAngrybird);
	BitBlt(g_mdc,g_iX,g_iY,70,70,g_bufdc,70,0,SRCAND);
	BitBlt(g_mdc,g_iX,g_iY,70,70,g_bufdc,0,0,SRCPAINT);

	//��g_mdc�е���������g_hdc��
	BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_mdc,0,0,SRCCOPY);

	//����X����ͼ�������ٶ�
	g_iX += g_iXSpeed;			//����X�᷽����ͼ���꣬ÿ����һ��Game_Paint������x����ͼ���һ���㶨�����g_iXSpeed���൱�������˶�
	g_iYSpeed = g_iYSpeed + g_iYGravity;		//����Y�᷽���ٶȷ�����g_iYSpeed����ÿһ��Game_Paint���������ĵ��þͼ���һ��g_iYGravity���������ٶȣ�
	g_iY += g_iYSpeed;			//����Y�᷽����ͼ���꣬ÿ����һ��Game_Paint������y����ͼ���һ���ոı�����g_iYSpeed���൱�ڼ����˶�

	//�ж��Ƿ񴥵أ�������������ڱ߽磬vy����Ϊ�෴����
	if(g_iY >= g_rect.bottom-70)     
	{
		g_iY= g_rect.bottom - 70;

		//X�᷽���Ħ��������
		g_iXSpeed += g_iXFriction;		//	vx=fx+vx������g_iXFrictionΪ��ֵ������ÿ����һ��Game_Paint������g_iXSpeed�㶨��Сһ��g_iXFriction����ֵ�Ĵ�С				
		if(g_iXSpeed < 0)		//��g_iXSpeedֵ�ݼ���С��0ʱ���ͽ�����Ϊ0����С����X�������ƶ���
			g_iXSpeed = 0;

		//Y�᷽��Ħ��������
		g_iYSpeed += g_iYFriction;      //vy=fy+vy������fyͬ��Ϊ��ֵ������ÿ����һ��Game_Paint������g_iYSpeed�㶨��Сһ��g_iYFriction�ľ���ֵ
		if(g_iYSpeed < 0)     //���ٶȼ���С�ڵ���0����Ϊ�㣬��С����X�������ƶ���
			g_iYSpeed = 0;
		//Y���ٶȴ�С���䣬������
		g_iYSpeed= -g_iYSpeed;
	}

	g_tPre = GetTickCount();     //��¼�˴λ�ͼʱ��
}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp( HWND hwnd )
{
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	DeleteObject(g_hAngrybird);
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd,g_hdc);
	return TRUE;
}