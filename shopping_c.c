#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Id Id;
typedef struct myItem myItem;

struct Id
{
	char id[30];
	char pass[30];
};

Id arr[1001];
int idIdx = 0,i;
int cur;

void loginMenu()
{
	printf("**************************************\n");
	printf("*         < 쇼핑몰 프로그램 >        *\n");
	printf("*            1. 로그인               *\n");
	printf("*            2. 아이디 생성          *\n");
	printf("*            3. 종료                 *\n");
	printf("**************************************\n");
}

void mainMenu(  )
{
	printf("환영합니다, %s 님!\n", arr[cur].id);
	printf("**************************************\n");
	printf("*         < 쇼핑몰 프로그램 >        *\n");
	printf("*            1. 내 장바구니          *\n");
	printf("*            2. 상품 보기            *\n");
	printf("*            3. 로그아웃             *\n");
	printf("**************************************\n");
}

void itemMenu(  )
{
	printf("**************************************\n");
	printf("*            < 상품목록 >            *\n");
	printf("*            1. 셔츠                 *\n");
	printf("*            2. 바지                 *\n");
	printf("*            3. 뒤로가기             *\n");
	printf("**************************************\n");
}

char Item[2][3][10]={
	{"셔츠", "박스티", "반팔티"},
	{"청바지", "반바지", "면바지"}
}; 

struct myItem
{
	int myItemNum;
	char name[30][30];
};

myItem my[30];
int i;
int main()
{
	for( i = 0 ; i < 30; i++)
		my[i].myItemNum = 0; 
	FILE *ID_FILE = fopen("id.txt", "r");
	if( ID_FILE == NULL )
	{
		fclose(ID_FILE);
		ID_FILE = fopen("id.txt", "w");
	}
	else
	{
		while( fscanf(ID_FILE, "%s%s", arr[idIdx].id, arr[idIdx].pass) == 2 ){ idIdx++;}
	}
	
	fclose(ID_FILE);
	
	while(1)
	{
		loginMenu();
		int sel;
		scanf("%d", &sel);
		if( sel == 1 )
		{
			char tempID[30];
			char tempPASS[30];
			printf("아이디를 입력하세요:");
			scanf("%s", tempID);
			printf("비밀번호를 입력하세요:");
			scanf("%s", tempPASS);
			cur = -1;
			for(  i = 0 ; i < idIdx ; i++)
			{
				if(!strcmp(tempID, arr[i].id) && !strcmp(tempPASS, arr[i].pass))
				{
					cur = i;
					break;
				}
			}
			
			if( cur == -1 )
			{
				printf("---! 아이디나 비밀번호를 다시 확인하세요 !---\n");
				printf("--아무키나 누르시오..-\n");
				getchar();
				getchar();
			}
			else
			{
				while(1)
				{
					system("cls");
					mainMenu(  );
					int sel2;
					scanf("%d", &sel2);
					
					if( sel2 == 1  )
					{
						printf("********************************************\n");
						printf("       <내 장바구니>                        \n");
						printf("주의: 장바구니 목록은 로그아웃시 사라집니다.\n"); 
						printf("--------------------------------------------\n");
						for( i = 0 ; i < my[cur].myItemNum ; i++)
						{
							printf("%d. %s\n", i+1, my[cur].name[i]);
						}
						printf("********************************************\n");
						printf("전부 구매하시려면 0, 아니면 아무키나 눌러주세요:");
						int s;
						scanf("%d", &s);
						if( s == 0 )
						{
							my[cur].myItemNum = 0;
							printf("전부 구매 완료되었습니다. 장바구니 목록은 삭제됩니다.\n");
						}
						printf("--아무키나 누르시오..\n");
						getchar();
						getchar(); 
					}
					else if( sel2 == 2 )
					{
						while(1)
						{
							system("cls");
							itemMenu();
							int sel3 ;
							scanf("%d", &sel3);
							
							if( sel3 == 1 )
							{
								system("cls");
								printf("*****************************\n");
								for( i = 0 ; i < 3; i ++)
									printf("        %d. %s\n", i+1, Item[sel3-1][i]);					
								printf("*****************************\n");
								printf("원하시는 번호를 입력하세요(뒤로가기는 0):");
								int sel4;
								scanf("%d", &sel4);
								
								if( sel4 == 0 )
								{
									printf("뒤로 돌아갑니다.\n");
								}
								else if( sel4 < 4 )
								{
									printf("[%s] 를장바구니에 담았습니다.\n", Item[sel3-1][sel4-1]);
									strcpy(my[cur].name[my[cur].myItemNum++], Item[sel3-1][sel4-1]);
								}
								else
								{
									printf("그런 번호는 없습니다.\n");
								}
								
								printf("--아무키나 누르시오..\n");
								getchar();
								getchar();
							}
							else if( sel3 == 2 )
							{
								system("cls");
								printf("*****************************\n");
								for( i = 0 ; i < 3; i ++)
									printf("        %d. %s\n", i+1, Item[sel3-1][i]);					
								printf("*****************************\n");
								printf("원하시는 번호를 입력하세요(뒤로가기는 0):");
								int sel4;
								scanf("%d", &sel4);
								
								if( sel4 == 0 )
								{
									printf("뒤로 돌아갑니다.\n");
								}
								else if( sel4 < 4 )
								{
									printf("[%s] 를장바구니에 담았습니다.\n", Item[sel3-1][sel4-1]);
									strcpy(my[cur].name[my[cur].myItemNum++], Item[sel3-1][sel4-1]);
								}
								else
								{
									printf("그런 번호는 없습니다.\n");
								}
								
								printf("--아무키나 누르시오..\n");
								getchar();
								getchar();
							}
							else if( sel3 == 3 )
							{
								break;
							}
						}
					}
					else if( sel2 == 3 )
					{
						printf("메인 화면으로 이동합니다.\n");
						printf("--아무키나 누르시오..--\n");
						getchar();
						getchar();
						break;
					}
				} 
			}
		}
		else if( sel == 2 )
		{
			char tempID[30];
			char tempPASS[30];
			printf("사용하실 아이디를 입력하세요:");
			scanf("%s", tempID);
			int check = 0;
			for(  i = 0 ; i < idIdx ; i++)
			{
				if(!strcmp(arr[i].id, tempID))
				{
					check = 1;
					break;
				}
			}
			
			if( check )
			{
				printf("아이디가 중복됩니다. 메인 화면으로 이동합니다.\n");
				printf("--아무키나 누르시오..\n");
				getchar();
				getchar();
			}
			
			printf("사용하실 비밀번호를 입력하세요:");
			scanf("%s", tempPASS);
			
			strcpy( arr[idIdx].id, tempID );
			strcpy( arr[idIdx++].pass, tempPASS); 
			
			printf("--------ID가 생성 완료되었습니다 !-----------\n");
			printf("--아무키나 누르시오..--\n");
			getchar();
			getchar();
		}
		else if( sel == 3 )
		{
			printf("프로그램이 종료됩니다.");
			
			ID_FILE = fopen("id.txt", "w");
			
			for( i = 0 ; i < idIdx; i++)
			{
				fprintf(ID_FILE,"%s\n%s\n", arr[i].id, arr[i].pass);
			}
			fclose(ID_FILE);
			break;
		}
		else
		{
			printf("번호를 다시 입력해주세요\n");
			printf("--아무 키나 누르시오..--\n");
			getchar();
			getchar();
		}
		
		system("cls");
	}
}
