#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

static	int tower[100][9],tower_road[100][9],tower_monster[100][9];//tower[層數][房間編號]存房間類型  tower_road存通道 player_ability存天賦 
	
static	int player_status[15]={1,100,50,30,10,20,10,7,50,0,100,50},pres_player_status[15]={1,100,50,30,10,20,10,7,50,0,100,50},skillnum=0,dust=0;//player_Status={0Level,1HP,2MP,3Atk,4def,5Matk,6Mdef,7Critical,8Rstealth,9EXP
						//				10MHP,11MMP,12mtkg,13defg,14matkg,15mdefg,}
static int buff_def_count=0,debuff_atk_count=0,debuff_poison_count=0;

static	char player_name[15];

static int player_position=8,pres_player_position=8,tower_level=0,kill_num=0;

static bool battling=false,just_up=false,player_ability[6],tower_monster_living[100][9];


struct skillinfo  //建立結構 
{
	int ID,type,MP;
	char name[100],description[100];
	bool learned;
}skill[100];

struct info  //建立結構 
{
	int HP,ATK,P_def,M_def,ID,attack_type;
	char name[100];
}monster[100],nowmonster1,pres_monster1;
	
	void skill1(){
		int i=rand()%20+150,cri=rand()%100+1;
		
		i=(player_status[3]*i/100)-nowmonster1.P_def;
		
		if(cri<=player_status[7]){
			
			i=i*2;
			printf("Critical Hit !\n");
		}
		
		if(i<0){
			i=0;
		}
		
		nowmonster1.HP-=i;
		
		printf("Use %s !!You deal %d damage to the monster!\n",skill[1].name,i);
	}
	void skill2(){
		
		int i=rand()%30+60;
		
		i=(player_status[3]*i/100)-nowmonster1.P_def;
		
		if(i<0){i=0;
		}
		
		nowmonster1.HP=nowmonster1.HP-i;
		player_status[1]=player_status[1]+(i/4);
		if(player_status[1]>player_status[10]){
			player_status[1]=player_status[10];
		}
		
		printf("Use %s !!You deal %d damage to the monster and drain %d HP!!!\n",skill[2].name,i,i/4);
		
		
	}
	void skill3(){
		
		int i=rand()%30+100;
		
		i=player_status[3]*i/100;
		
		nowmonster1.HP-=i;
		
		printf("Use %s !!The enemy receive %d damage without any resistance!\n",skill[3].name,i);
	}
	void skill4(){
		
		int i=rand()%5+2;
		
		i=dust/100*i;
		
		nowmonster1.HP-=i;
		
		printf("You throw %d dust to the enemy and deal %d damage,how waste!\n",dust/100,i);
		
		dust=dust-dust/100;
	}
	void skill5(){
		
		int i=rand()%100+5;
		
		if(i<=5){
			
			nowmonster1.HP=0;
			
			printf("You cut the monster into two pieces!\n");
			
		}
		else{
			printf("Failed!You have no chance to attack the enemy!\n");
		}
			
	}
	void skill6(){
		
		int i=rand()%25+85,cri=rand()%100-34;
		
		i=(player_status[3]*i-nowmonster1.P_def)/100;
		
		if(cri<=player_status[7]){
			
			i=i*2;
			printf("Critical Hit !\n");
		}
		
		if(i<0){
			i=0;
		}
		nowmonster1.HP-=i;
		printf("You attack the monster vigorously!!Deal %d damage!\n",i);
		
	}
	void skill7(){
		
		int i=(player_status[10]-player_status[1])*(rand()%35+15)/100;
		
		player_status[1]+=i;
		
		printf("You pray for your god...God Blessing!You recover %d HP!\n",i);
		
		
	}
	void skill8(){
		
		if(buff_def_count==0){
			
		pres_player_status[4]=player_status[4];
		
		player_status[4]+=(7+player_status[0]*2);
		
			
		}
		
		printf("You become harder!Your defense has increased %d.\n",7+player_status[0]*2);
		
		buff_def_count=2;
		
	}
	void skill9(){
		
		if(debuff_atk_count==0){
			
		pres_monster1.ATK=nowmonster1.ATK;
		
		nowmonster1.ATK-=(5+player_status[0]*2);
			
			
		}
		
	
		
		printf("The monster become weaken!It's power is down about %d\n",5+player_status[0]*2);
		
		debuff_atk_count=2;
		
		
	}
	void skill10(){
		
		int i=rand()%50+160;
		
		i=player_status[5]*i/100-nowmonster1.M_def;
		
		if(i<0){i=0;
		}
		
		nowmonster1.HP-=i;
		
		printf("A big Fireball!!You deal %d damage to the enemy!\n",i);
		
		
	}
	void skill11(){
			
		int i=(player_status[11]-player_status[2])*(rand()%35+15)/100;
		
		player_status[2]+=i;
		
		printf("You meditation for a while.Fresh!You recover %d mana!\n",i);
		
		
	}
	void skill12(){
			debuff_poison_count++;
		printf("The enemy is poisoned!");
	}
	
	void (*skill_call[12])()={skill1,skill2,skill3,skill4,skill5,skill6,skill7,skill8,skill9,skill10,skill11,skill12};	
   

	
int main(int argc, char *argv[]) {
	
	srand(time(NULL));
	
	player_creat();
	
	skill_load();
	
	monster_creat();
	
    tower_creat();
    
    system("cls");
    
	
	while(player_status[1]>0){
		
		map_print();
    
    status_print();
    
	situation_print();
   
		if(player_status[9]>=player_status[0]+4){
		player_levelup();
	}


    if(tower_monster_living[tower_level][player_position-1]==true){
    	monster_command();
    	if(player_status[9]>=player_status[0]+4){
		player_levelup();
	}
	}
	
	 
    command_input();

	system("cls");
	}
	system("cls");
    printf("You DIED.\nYour score is : %d\n",tower_level*10+player_status[0]*5+kill_num*2);
    
    FILE *fp = fopen("savedata/scores.txt", "a+");
				  time_t  timer = time(NULL);
			fprintf(fp,"\t%15s score = %d\t\t%s\n\n",player_name,tower_level*10+player_status[0]*5+kill_num*2,ctime(&timer));


		
    	fclose(fp);
    
    system("pause");
    
    
	return 0;
}

void player_creat(){
	
	int point=1;
	
	char choose[2]={'0'};
	
	printf("Please enter Player name(Max 15 words):");
	
	scanf("%s",&player_name);
	
	while(point>0){
	
	printf("Please choose your ability(%dleft):\n(1)怪力 (你天生神力，開場即會使用重斬)\n(2)鷹眼 (你目光如炬，可以輕易看破怪物的狀態)\n(3)輕靈 (你身輕如燕，躲過怪物的眼睛輕而易舉)\n(4)技能78\n(5)火之子 (你是火的孩子！開場即會使用火球術)\n(6)赫爾魯斯之血 (你繼承了傳說中的血統，當你受傷時會逐漸回復)\n",point);
	
	unability: scanf("%s",&choose);

	switch(choose[0]){
		
		case '1':{
			
			skill[1].learned=true;
			
			break;
		}
		case '2':{
			player_ability[1]=true;
			
			break;
		}
		case '3':{
			player_ability[2]=true;
			break;
		}
		case '4':{
			skill[8].learned=true;
			skill[9].learned=true;
			
			break;
		}
		case '5':{
			skill[10].learned=true;
			break;
		}
		case '6':{
			player_ability[0]=true;
			break;
		}
		default:printf("Not this number!\n");goto unability;
	}
	
	point--;
				   }
				
	point=3;
	
	while(point!=0){
		
	system("cls");
	
	printf("Please choose your bonus(%dleft):\n",point);
	
	printf("(1)+%dHP\n(2)+%dMP\n(3)+%dAtk\n(4)+%dDef\n(5)+%dMAtk\n(6)+%dMDef\n(7)+%dCrit\n",30/(4-point),12/(4-point),6/(4-point),6/(4-point),6/(4-point),6/(4-point),9/(5-point));
	
	printf("Your status now:HP=%d/%d MP=%d/%d Atk=%d Def=%d MAtk=%d MRes=%d Crit=%d\n",player_status[1],player_status[1],player_status[2],player_status[2],player_status[3],player_status[4],player_status[5],player_status[6],player_status[7],player_status[8]);
	
	unstatus:scanf("%s",&choose);
	
	switch(choose[0]){
		
		case '1':{
			player_status[1]=player_status[1]+30/(4-point);
			player_status[10]=player_status[10]+30/(4-point);
			break;
		}
		case '2':{
			player_status[2]=player_status[2]+12/(4-point);
			player_status[11]=player_status[11]+12/(4-point);
			break;
		}
		case '3':{
			player_status[3]=player_status[3]+6/(4-point);
			break;
		}
		case '4':{
			player_status[4]=player_status[4]+6/(4-point);
			break;
		}
		case '5':{
			player_status[5]=player_status[5]+6/(4-point);
			break;
		}
		case '6':{
			player_status[6]=player_status[6]+6/(4-point);
			break;
		}
		case '7':{
			player_status[7]=player_status[7]+9/(5-point);
			break;
		}
		default:printf("NO this number!!\n");goto  unstatus;
	}
	
	point--;
		
				   }
}





void tower_creat(){
	
	int i=0,y=0,randnum=0;
	
	while(i<100){
		
		y=0;
		
		while(y<9){
		
		tower[i][y]=(rand()%6)+1;
		
		tower_road[i][y]=(rand()%3)+1;
		
		if(tower[i][y]>2){
			tower_monster_living[i][y]=true;
		}
		
		y++;
			  
				}
				
			randnum=(rand()%9);
			
			tower[i][randnum]=0;
			
 			i++;
				
			    
				}
	
}



void map_print(){//塗地圖 
	int n=0,twos=0,coun=0;
	while(n<2){
	printf("  ----    ----    ----  \n");
	if(tower_road[tower_level][1+coun]<3){
		if(player_position==1+coun){	
			printf(" |  O |--");
		}
		else{
			printf(" |    |--");
		}
	}
	else{
		if(player_position==1+coun){	
			printf(" |  O |  ");
		}
		else{
			printf(" |    |  ");
		}
		
	}if(tower_road[tower_level][2+coun]<3){
		if(player_position==2+coun){	
			printf("|  O |--");
		}
		else{
			printf("|    |--");
		}
	}
	else{
		if(player_position==2+coun){	
			printf("|  O |  ");
		}
		else{
			printf("|    |  ");
		}
		
	}
	
		if(player_position==3+coun){
			printf("|  O |  \n");
		}
		else{
			printf("|    |  \n");
		}
	
	printf("  ----    ----    ----  \n");
	
	if(tower_road[tower_level][1+coun]>1){
		printf("   |       ");
	}
	else{
		printf("           ");
	}
	if(tower_road[tower_level][2+coun]>1){
		printf("|       ");
	}
	else{
		printf("        ");
	}
	printf("|\n");
	n++;
	coun=3;
	}
	
	
	printf("  ----    ----    ----  \n");
	if(player_position==7){
		printf(" |  O |--");
	}
	else{
		printf(" |    |--");
	}
	if(player_position==8){
		printf("|  O |--");
	}
	else{
		printf("|    |--");
	}
	if(player_position==9){
		printf("|  O |\n");
	}
	else{
		printf("|    |\n");
	}	
		printf("  ----    ----    ----  \n");
	}

	void status_print(){
		printf("===========================================================================================================\n");
		printf(" %s LV.%d  HP=%d/%d MP=%d/%d Atk=%d Def=%d MAtk=%d MRes=%d Crit=%d EXP=%d/%d   第%d層  %dDust\n",player_name,player_status[0],player_status[1],player_status[10],player_status[2],player_status[11],player_status[3],player_status[4],player_status[5],player_status[6],player_status[7],player_status[9],player_status[0]+4,tower_level+1,dust);
		printf("===========================================================================================================\n");
	}

	void command_input(){
		
		int n=0;
		
		char input[2]={'0'};
		
		invalid_command:printf("What to do now?\n");
		
		scanf("%s",&input);
		
		
		if(input[0]=='a'){
		
		printf("yes");
	
	}
		
		switch(input[0]){
			
			case '8':{
				
			if(player_position>3 && tower_road[tower_level][player_position-3]>1){
				pres_player_position=player_position;
				player_position-=3;
				just_up=false;
			}
			else if(player_position%3==0 && player_position!=3){
				pres_player_position=player_position;
				player_position-=3;
				just_up=false;
				
			}
			else{
				printf("Invalid path!\n");
				goto invalid_command;
			}
				break;
			}

			case '4':{
				
			if(player_position%3!=1 && player_position<7 && tower_road[tower_level][player_position-1]<3){
				pres_player_position=player_position;
				player_position=player_position-1;
				just_up=false;
			}
			else if(player_position>7){
				pres_player_position=player_position;
				player_position-=1;
				just_up=false;
			}
			else{
				printf("Invalid path!\n");
				goto invalid_command;
			}
				break;
			}
			case '6':{
			
			if(player_position%3!=0 && tower_road[tower_level][player_position]<3){
				pres_player_position=player_position;
				player_position+=1;
				just_up=false;
			}
			else if(player_position>6 && player_position<9){
				pres_player_position=player_position;
				player_position+=1;
				just_up=false;
			}
			else{
				printf("Invalid path!\n");
				goto invalid_command;
			}
				break;
			}
			case '2':{
			if(player_position<7 && tower_road[tower_level][player_position]>1){
				pres_player_position=player_position;
				player_position+=3;
				just_up=false;
			}
			else if(player_position%3==0 && player_position!=9){
				pres_player_position=player_position;
				player_position+=3;
				just_up=false;
				
			}
			else{
				printf("Invalid path!\n");
				goto invalid_command;
			}
				break;
			}
			case 'u':{
				
				if(tower[tower_level][player_position-1]==0){
					
					tower_level++;
					
					player_status[9]++;
					
					just_up=true;
					if(player_status[9]>=player_status[0]+4){
					player_levelup();
															}
					
					pres_player_position=player_position;
				}
				else{
				printf("No way to get up!\n");
				goto invalid_command;
			}
				
				break;
			}
			
	}
}

void situation_print(){
	
	if(tower[tower_level][player_position-1]==0){
		
		printf("你看到通往樓上的階梯\n");
		
	}
	else if(tower[tower_level][player_position-1]==1){
		
		printf("這是普通的小房間，你不認為這裡有什麼值得探索的東西\n");
		
	}
	else if(tower[tower_level][player_position-1]==2){
		
		printf("空氣中飄著陳腐的味道......是書！這裡是書房！\n");
		
	}else if(tower[tower_level][player_position-1]==3){
		
		printf("房間的中央矗立著不祥的雕像，你盯著他，感覺自己快要發狂\n");
		
	}else if(tower[tower_level][player_position-1]==4){
		
		printf("寶物庫！你的辛苦總算有了代價！\n");
		
	}else if(tower[tower_level][player_position-1]==5){
		
		printf("很香的味道......這裡應該是廚房。\n你不由自主地偷看了一下大釜中的食物......噁，是人肉！\n");
		
	}
	else {
		
		printf("這個房間空蕩蕩的，已被搜刮的一乾二淨\n");
		
	}
	
}

void monster_creat(){
	
	int i=2,a=0,random=rand()%20+1;
	char info[100];
	
	FILE *fp;

       if( ( fp = fopen( "data/monsters.csv", "r")) == NULL) //沒找到檔案就結束 
       {
              puts("Cannot open the file");
       }
       
       while (a==0) //開始一行一行往下，從檔案讀進info字串 
		{
            
            while(fgets(info, sizeof(info), fp) && i<20)  //當讀到第random行時，要指定怪的話就改成ID+1(跳過標題標示) 
            {
				sscanf(info,"%d,%d,%d,%d,%d,%d,%[^\n]",&monster[i-2].ID,&monster[i-2].HP,&monster[i-2].ATK,&monster[i-2].P_def,&monster[i-2].M_def,&monster[i-2].attack_type,&monster[i-2].name);  
                i++;
			}
			a++;
        }
     
        fclose(fp);
        i=0,a=0;
        
        while(i<100){
        	
        	while(a<9){
        		
        		tower_monster[i][a]=ceil((rand()%16+1)*(i+1)/30)+1;
        		
        		if(tower_monster[i][a]>16){
        			tower_monster[i][a]=16;
				}
        		
        		a++;
			}
        	i++;
        	
        	a=0;
        	
		}
}

void battle(){

	map_print();
	status_print();
	
	int i=2,a=0,random,n=0,compare,turn=1;
	static int which_skills[15];	
	pres_monster1=nowmonster1;
	
	if(player_ability[1]==true){
		
		printf("The %s (HP=%d) appear!\n",nowmonster1.name,nowmonster1.HP);
	}
	else{
		printf("The %s appear!\n",nowmonster1.name);
	}
	
	i=0;
	
	int damage,id,count=0;
	
	char command[1];
	
	if(nowmonster1.HP<=0 || player_status[1]<=0){battling=false;}
	
	else{
		
		battle_end:random=0;
		
		while(i==0){printf("============\n  TURN %d\n============\n",turn);
			
		invalid_command: printf("Your turn ! What to do now ?\n");
		
		 scanf("%s",&command[0]);
			
			if(command[0]=='k'){system("cls");map_print();
				status_print();printf("============\n  TURN %d\n============\n",turn);
				printf("Attack!\n");
				damage=player_damage_count();	
				nowmonster1.HP=nowmonster1.HP-damage;
				printf("You deal %d damage to the monster!\n",damage);
			}
			else if(command[0]=='r'){
				
				random=rand()%100+1;
				
				if(random<=40&&just_up!=true){
					
					player_position=pres_player_position;
					system("cls");map_print();
				status_print();printf("============\n  TURN %d\n============\n",turn);
					
					printf("你逃走了！\n");
					
					battling=false;
					
					i++;
					
					goto battle_end;
					
				}
				else if(just_up==true){system("cls");map_print();
				status_print();printf("============\n  TURN %d\n============\n",turn);
					printf("無路可逃！\n");
				}
				else{system("cls");map_print();
				status_print();printf("============\n  TURN %d\n============\n",turn);
					
				printf("逃跑失敗！\n");
					}
				
			}else if(command[0]=='s'){
				printf("Use which skill?\n");
				
				id=1,count=0;
				
				while(id<12){
					
				if(skill[id].learned==true){
						
					printf("(%d) %s───  %s(%d)\n",count+1,skill[id].name,skill[id].description,skill[id].MP);
					
					which_skills[count]=id;
					count++;
											}
					id++;	
							}
				printf("(b)back\n");
				
				invalid_skill_command: scanf("%s",&command[0]);	
				
				if(command[0]=='b'){
					
					goto invalid_command;
					
				}
				else if(count!=0){
				compare=atoi(command);
				
				if(compare<=count && player_status[2]>=skill[which_skills[compare-1]].MP){
					system("cls");map_print();
				status_print();printf("============\n  TURN %d\n============\n",turn);
	
				player_status[2]=player_status[2]-skill[which_skills[compare-1]].MP;
				
				skill_call[(skill[which_skills[compare-1]].ID)-1]();

					
				count=0,compare=0;
				}
				else if(compare<=count && player_status[2]<skill[which_skills[compare-1]].MP){
					
					printf("You don't have enough mana!\n'");
					goto invalid_command;
				}
				else{
					printf("Invalid command!\n");
					goto invalid_command;
				}
				}
					
				}
				else{printf("Invalid command!\n");goto invalid_command;
				}
		if(debuff_poison_count!=0){
			nowmonster1.HP=nowmonster1.HP-(debuff_poison_count*5/100*nowmonster1.HP);		}
				
				
		if(nowmonster1.HP<=0){
			printf("The monster is dead!\n");
			tower_monster_living[tower_level][player_position-1]=false;
			battling=false;
			player_status[9]++;
			
			int gold=(tower_level+1)*(rand()%5+5);
			dust=dust+gold;
			
			printf("You gain %d dust.\n",gold);
			i++;
			debuff_poison_count=0;
			debuff_atk_count=0;
			buff_def_count=0;
			kill_num++;
		}
		else{
			
			turn++;
			
			if(player_ability[1]==true){
				
				printf("%s (HP=%d) attack you!\n",nowmonster1.name,nowmonster1.HP);
			}
			else{
			printf("%s attack you!\n",nowmonster1.name);
				
			}
		
			damage=monster_damage_count();
			
			printf("The monster deal %d damage to you!\n");
			
			player_status[1]=player_status[1]-damage;
			
			buff_print();

		}
		if(player_status[1]<=0){
		i++;}
		if(player_ability[0]==true&&player_status[1]!=player_status[10]){
			player_status[1]+=player_status[10]*2/100;
			if(player_status[1]>player_status[10]){
				player_status[1]=player_status[10];
			}
			printf("Your blood is heating!Your body is recovering\n");
		}

			}
			if(tower_monster_living[tower_level][player_position-1]==true){
    	monster_command();
    	if(player_status[9]>=player_status[0]+4){
		player_levelup();
	}
	}
						}
		
		}
		
	
			
		
	
	


	int player_damage_count(){
		
		int i=rand()%25+85,cri=rand()%100+1;
		
		i=(player_status[3]*i-nowmonster1.P_def)/100;
		
		if(cri<=player_status[7]){
			
			i=i*2;
			printf("Critical Hit !\n");
		}
		
		if(i<0){
			i=0;
		}
		
		return i;
		
	}
	int monster_damage_count(){
		
		int i=rand()%60+60;
		
		if(nowmonster1.attack_type==1){
			i=(nowmonster1.ATK*i-player_status[4])/100;
		}
		else{
			i=(nowmonster1.ATK*i-player_status[6])/100;
		}
		
		if(i<0){
			i=0;
		}
		
		return i;
		
	}
	
	void player_levelup(){
		
		player_status[0]++;
		player_status[9]=0;
		player_status[1]=player_status[10];
		player_status[2]=player_status[11];
		player_status[1]=player_status[1]+15;
	player_status[10]=player_status[10]+15;
	player_status[2]=player_status[2]+5;
	player_status[11]=player_status[11]+5;
	player_status[3]=player_status[3]+3;
	player_status[4]=player_status[4]+3;
	player_status[5]=player_status[5]+3;
	player_status[6]=player_status[6]+3;
	player_status[7]=player_status[7]+1;
		int	point=3;
		
		char choose[2];
	
	while(point!=0){
	
	system("cls");
	
	
	printf("Level UP!\nPlease choose your bonus(%dleft):\n",point);
	
	printf("(1)+%dHP\n(2)+%dMP\n(3)+%dAtk\n(4)+%dDef\n(5)+%dMAtk\n(6)+%dMDef\n(7)+%dCrit\n",10,4,2,2,2,2,1);
	
	printf("Your status now:HP=%d/%d MP=%d/%d Atk=%d Def=%d MAtk=%d MRes=%d Crit=%d\n",player_status[1],player_status[10],player_status[2],player_status[11],player_status[3],player_status[4],player_status[5],player_status[6],player_status[7],player_status[8]);
	
	unstatus:scanf("%s",&choose);
	
	
	
	switch(choose[0]){
		
		case '1':{
			player_status[1]=player_status[1]+10;
			player_status[10]=player_status[10]+10;
			break;
		}
		case '2':{
			player_status[2]=player_status[2]+4;
			player_status[11]=player_status[11]+4;
			break;
		}
		case '3':{
			player_status[3]=player_status[3]+2;
			break;
		}
		case '4':{
			player_status[4]=player_status[4]+2;
			break;
		}
		case '5':{
			player_status[5]=player_status[5]+2;
			break;
		}
		case '6':{
			player_status[6]=player_status[6]+2;
			break;
		}
		case '7':{
			player_status[7]=player_status[7]+1;
			break;
		}
		default:printf("NO this number!!\n");goto  unstatus;
	}
	
	point--;
		
				   }
	system("cls");
	
	if(player_status[0]%5==0){
		
		int random=rand()%11,which_skill[3],choose_what;
		
		printf("Please choose a skill you want to learn!\n");
		int i=0;
		while(i<3){
			if(skill[random].learned==false){
			
			printf("(%d)%s─── %s (%d)\n",i+1,skill[random].name,skill[random].description,skill[random].MP);
			
			which_skill[i]=skill[random].ID;
			
			i++;
		}
		random=rand()%11;
		}
		invalid_choice: scanf("%s",choose);
		
		choose_what=atoi(choose);
		
		if(choose_what==1){
			skill[which_skill[0]].learned=true;
		}
		else if(choose_what==2){
			skill[which_skill[1]].learned=true;
		}
		else if(choose_what==3){
			skill[which_skill[2]].learned=true;
		}
		else{
			printf("Invalid number!\n");
			goto invalid_choice;
		}
		
		system("cls");
	}
		
	map_print();
    
    status_print();
    
	situation_print();
	}
	
	void skill_load(){
		
	int i=2,a=0;
	char temp[100];
	FILE *fp;

       if( ( fp = fopen( "data/Skills.csv", "r")) == NULL) //沒找到檔案就結束 
       {
              puts("Cannot open the file");
       }
       
       while (a==0) //開始一行一行往下，從檔案讀進info字串 
		{
            while(fgets(temp, sizeof(temp), fp) && i<15)  //當讀到第random行時，要指定怪的話就改成ID+1(跳過標題標示) 
            {
				sscanf(temp,"%d,%d,%d,%[^,],%[^\n]",&skill[i-2].ID,&skill[i-2].type,&skill[i-2].MP,skill[i-2].description,skill[i-2].name);  
                i++;
                
				//使用isscanf將info字串內的東西格式化(類似分割)，並依序存入結構 
			}
			a++;
        }
     
        fclose(fp);
	}

void buff_print(){
	
	if(debuff_atk_count!=0){
				debuff_atk_count--;
				if(debuff_atk_count==0){
		nowmonster1.ATK=pres_monster1.ATK;
		printf("The monster's power is back to normal!\n");
								}		
				else{
		printf("The monster is weaken! left %d turn\n",debuff_atk_count);
					}
			}
	if(buff_def_count!=0){
				buff_def_count--;
				if(buff_def_count==0){
					player_status[4]=pres_player_status[4];
					printf("Your body is back to normal!Be careful!\n");
	}
	else{
		printf("You are Harded than normal! left %d turn\n",buff_def_count);
	}
			}
	
	
	
	
	
}
	
void monster_command(){
	nowmonster1=monster[tower_monster[tower_level][player_position-1]];
	
	if(player_ability[1]==true){
		printf("You see a %s (HP=%d) in the room.What do you want to do?\n(1)Battle\n(2)Search\n(3)Go back\n",nowmonster1.name,nowmonster1.HP);
	}
	else{
		printf("You see a %s  in the room.What do you want to do?\n(1)Battle\n(2)Search\n(3)Go back\n",nowmonster1.name);
	}
	char monster_command_input[2];
	
	invalid_monster_command: scanf("%s",&monster_command_input[0]);
	
	switch(monster_command_input[0]){
		case '1':{
			battling=true;
			system("cls");
			battle();
			break;
		}
		case '2':{
			int spot=rand()%100+1;
			if(player_ability[2]==true){
				spot-=20;
			}
			if(spot<=35){
				printf("Success! The monster doesn't notice you!But you can still be spotted if you stay in this room....\n");
			}
			else{
				system("cls");
				printf("Failed!You are spotted by the monster!\n");
				int damages=monster_damage_count();
				printf("The monster deal %d damage to you!\n",damages);
				player_status[1]-=damages;
				battling=true;
				battle();
			}
			break;
		}
		case '3':{
			
			if(player_position!=pres_player_position&& just_up!=true){
				player_position=pres_player_position;
			system("cls");
			map_print();
			status_print();
			situation_print();
			}
			else{
				printf("你本來就在這個房間裡了！\n");
				goto invalid_monster_command;
			}
			
			if(tower_monster_living[tower_level][player_position-1]==true){
				monster_command();
			}
			break;
		}
		default:printf("Invalid command!\n");goto invalid_monster_command;

	}
	
}
	
