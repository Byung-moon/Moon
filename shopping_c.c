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
	printf("*         < ���θ� ���α׷� >        *\n");
	printf("*            1. �α���               *\n");
	printf("*            2. ���̵� ����          *\n");
	printf("*            3. ����                 *\n");
	printf("**************************************\n");
}

void mainMenu(  )
{
	printf("ȯ���մϴ�, %s ��!\n", arr[cur].id);
	printf("**************************************\n");
	printf("*         < ���θ� ���α׷� >        *\n");
	printf("*            1. �� ��ٱ���          *\n");
	printf("*            2. ��ǰ ����            *\n");
	printf("*            3. �α׾ƿ�             *\n");
	printf("**************************************\n");
}

void itemMenu(  )
{
	printf("**************************************\n");
	printf("*            < ��ǰ��� >            *\n");
	printf("*            1. ����                 *\n");
	printf("*            2. ����                 *\n");
	printf("*            3. �ڷΰ���             *\n");
	printf("**************************************\n");
}

char Item[2][3][10]={
	{"����", "�ڽ�Ƽ", "����Ƽ"},
	{"û����", "�ݹ���", "�����"}
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
			printf("���̵� �Է��ϼ���:");
			scanf("%s", tempID);
			printf("��й�ȣ�� �Է��ϼ���:");
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
				printf("---! ���̵� ��й�ȣ�� �ٽ� Ȯ���ϼ��� !---\n");
				printf("--�ƹ�Ű�� �����ÿ�..-\n");
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
						printf("       <�� ��ٱ���>                        \n");
						printf("����: ��ٱ��� ����� �α׾ƿ��� ������ϴ�.\n"); 
						printf("--------------------------------------------\n");
						for( i = 0 ; i < my[cur].myItemNum ; i++)
						{
							printf("%d. %s\n", i+1, my[cur].name[i]);
						}
						printf("********************************************\n");
						printf("���� �����Ͻ÷��� 0, �ƴϸ� �ƹ�Ű�� �����ּ���:");
						int s;
						scanf("%d", &s);
						if( s == 0 )
						{
							my[cur].myItemNum = 0;
							printf("���� ���� �Ϸ�Ǿ����ϴ�. ��ٱ��� ����� �����˴ϴ�.\n");
						}
						printf("--�ƹ�Ű�� �����ÿ�..\n");
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
								printf("���Ͻô� ��ȣ�� �Է��ϼ���(�ڷΰ���� 0):");
								int sel4;
								scanf("%d", &sel4);
								
								if( sel4 == 0 )
								{
									printf("�ڷ� ���ư��ϴ�.\n");
								}
								else if( sel4 < 4 )
								{
									printf("[%s] ����ٱ��Ͽ� ��ҽ��ϴ�.\n", Item[sel3-1][sel4-1]);
									strcpy(my[cur].name[my[cur].myItemNum++], Item[sel3-1][sel4-1]);
								}
								else
								{
									printf("�׷� ��ȣ�� �����ϴ�.\n");
								}
								
								printf("--�ƹ�Ű�� �����ÿ�..\n");
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
								printf("���Ͻô� ��ȣ�� �Է��ϼ���(�ڷΰ���� 0):");
								int sel4;
								scanf("%d", &sel4);
								
								if( sel4 == 0 )
								{
									printf("�ڷ� ���ư��ϴ�.\n");
								}
								else if( sel4 < 4 )
								{
									printf("[%s] ����ٱ��Ͽ� ��ҽ��ϴ�.\n", Item[sel3-1][sel4-1]);
									strcpy(my[cur].name[my[cur].myItemNum++], Item[sel3-1][sel4-1]);
								}
								else
								{
									printf("�׷� ��ȣ�� �����ϴ�.\n");
								}
								
								printf("--�ƹ�Ű�� �����ÿ�..\n");
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
						printf("���� ȭ������ �̵��մϴ�.\n");
						printf("--�ƹ�Ű�� �����ÿ�..--\n");
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
			printf("����Ͻ� ���̵� �Է��ϼ���:");
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
				printf("���̵� �ߺ��˴ϴ�. ���� ȭ������ �̵��մϴ�.\n");
				printf("--�ƹ�Ű�� �����ÿ�..\n");
				getchar();
				getchar();
			}
			
			printf("����Ͻ� ��й�ȣ�� �Է��ϼ���:");
			scanf("%s", tempPASS);
			
			strcpy( arr[idIdx].id, tempID );
			strcpy( arr[idIdx++].pass, tempPASS); 
			
			printf("--------ID�� ���� �Ϸ�Ǿ����ϴ� !-----------\n");
			printf("--�ƹ�Ű�� �����ÿ�..--\n");
			getchar();
			getchar();
		}
		else if( sel == 3 )
		{
			printf("���α׷��� ����˴ϴ�.");
			
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
			printf("��ȣ�� �ٽ� �Է����ּ���\n");
			printf("--�ƹ� Ű�� �����ÿ�..--\n");
			getchar();
			getchar();
		}
		
		system("cls");
	}
}
