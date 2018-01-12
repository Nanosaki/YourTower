#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define max_length_of_name 50

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

static	int tower[100][9],tower_road[100][9],tower_monster[100][9];//tower[層數][房間編號]存房間類型  tower_road存通道 player_ability存天賦 
	
static	int player_status[15]={1,100,50,30,10,20,10,7,50,0,100,50,2,100},pres_player_status[15]={1,100,50,30,10,20,10,7,50,0,100,50},skillnum=0,dust=0;//player_Status={0Level,1HP,2MP,3Atk,4def,5Matk,6Mdef,7Critical,8Rstealth,9EXP
						//				10MHP,11MMP,12range,13speed}
static int buff_def_count=0,debuff_atk_count=0,debuff_poison_count=0,battle_distance=3,amount_of_equipments=40;

static	char player_name[15];

static int player_position=8,pres_player_position=8,tower_level=0,kill_num=0,max_item=0;

static bool battling=false,just_up=false,player_ability[6],tower_monster_living[100][9];

void leader_of_village(void); 
void teleporter(void);
void seller_of_potion(void);
void seller_of_equipment(void);

struct armor{
	int P_Def,distance; //物防 
	int M_Def; //魔防
	int atk; //血量 
	int matk,type; //魔力 
	char name[max_length_of_name]; //裝備名稱 
	int ID; //裝備ID 
	int price;
	bool have; //是否擁有裝備 
	bool is_weared; //是否穿戴裝備 
}equipments[60],equ_in_bag[7],equ_weared[5];

struct skillinfo  //建立結構 
{
	int ID,type,MP,distance;
	char name[100],description[100];
	bool learned;
}skill[30];

struct info  //建立結構 
{
	int HP,ATK,P_def,M_def,ID,attack_type,attack_distance,speed;
	char name[100];
}monster[40],nowmonster1,pres_monster1;
	
	void skill1(){
		int i=rand()%20+150,cri=rand()%100+1;
		
		i=(player_status[3]*i/100)-nowmonster1.P_def;
		
		if(cri<=player_status[7]){
			
			i=i*2;
			printf("爆擊!\n");
		}
		
		if(i<0){
			i=0;
		}
		
		nowmonster1.HP-=i;
		
		printf("使用 %s !!你對敵人造成 %d 點傷害!\n",skill[2].name,i);
	}
	void skill2(){
		
		int i=rand()%15+100;
		
		i=(player_status[3]*i/100)-nowmonster1.P_def;
		
		if(i<0){i=0;
		}
		
		nowmonster1.HP=nowmonster1.HP-i;
		player_status[1]=player_status[1]+(i/2);
		if(player_status[1]>player_status[10]){
			player_status[1]=player_status[10];
		}
		
		printf("使用 %s !!你對怪物造成 %d 點傷害並吸取 %d 點生命!!!\n",skill[3].name,i,i/2);
		
		
	}
	void skill3(){
		
		int i=rand()%30+100;
		
		i=player_status[3]*i/100;
		
		nowmonster1.HP-=i;
		
		printf("使用 %s !!怪物在無法抵抗的狀況下受到 %d 點傷害!\n",skill[4].name,i);
	}
	void skill4(){
		
		int i=rand()%5+2;
		
		i=dust/100*i;
		
		nowmonster1.HP-=i;
		
		printf("你引爆了 %d 份的塵並造成 %d 點傷害，多麼奢侈!\n",dust/100,i);
		
		dust=dust-dust/100;
	}
	void skill5(){
		
		int i=rand()%100+8;
		
		if(i<=5){
			
			nowmonster1.HP=0;
			
			printf("你將怪物切成兩半!\n");
			
		}
		else{
			printf("失敗，怪物毫無破綻!\n");
		}
			
	}
	void skill6(){
		
		int i=rand()%25+85,cri=rand()%100-34;
		
		i=(player_status[3]*i-nowmonster1.P_def)/100;
		
		if(cri<=player_status[7]){
			
			i=i*2;
			printf("爆擊!\n");
		}
		
		if(i<0){
			i=0;
		}
		nowmonster1.HP-=i;
		printf("你精確的瞄準了怪物的弱點，造成 %d 點傷害!\n",i);
		
	}
	void skill7(){
		
		int i=(player_status[10]-player_status[1])*(rand()%35+15)/100;
		
		player_status[1]+=i;
		
		printf("你虔誠的向神祈禱......神明保佑！你回復了 %d 點血量!\n",i);
		
		
	}
	void skill8(){
		
		if(buff_def_count==0){
			
		pres_player_status[4]=player_status[4];
		
		player_status[4]+=(7+player_status[0]*2);
		
			
		}
		
		printf("你變得更耐打了，你的防禦力上升了 %d 點！\n",7+player_status[0]*2);
		
		buff_def_count=2;
		
	}
	void skill9(){
		
		if(debuff_atk_count==0){
			
		pres_monster1.ATK=nowmonster1.ATK;
		
		nowmonster1.ATK-=(5+player_status[0]*2);
			
			
		}
		
	
		
		printf("怪物變得虛弱了！牠的攻擊力下降了約 %d\n",5+player_status[0]*2);
		
		debuff_atk_count=2;
		
		
	}
	void skill10(){
		
		int i=rand()%50+160;
		
		i=player_status[5]*i/100-nowmonster1.M_def;
		
		if(i<0){i=0;
		}
		
		nowmonster1.HP-=i;
		
		printf("一顆大火球！你對怪物造成了 %d 點傷害！\n",i);
		
		
	}
	void skill11(){
			
		int i=(player_status[11]-player_status[2])*(rand()%35+15)/100;
		
		player_status[2]+=i;
		
		printf("你冥想了一下，回復 %d 點魔力！\n",i);
		
		
	}
	void skill12(){
			debuff_poison_count++;
		printf("敵人中毒了！\n");
	}
	
	void (*skill_call[12])()={skill1,skill2,skill3,skill4,skill5,skill6,skill7,skill8,skill9,skill10,skill11,skill12};	
   

	
int main(int argc, char *argv[]) {
	
	srand(time(NULL));
	
	player_creat();
	
	skill_load();
	
	equipments_load();
	
	monster_creat();
	
    tower_creat();
    
    system("cls");
    
    equipment_display();
    
	
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
	if(player_status[1]<=0){
		break;
	}
    command_input();
	system("cls");
	}
	system("cls");
    printf("You DIED.\nYour score is : %d\n",tower_level*10+player_status[0]*5+kill_num*2+dust/100);
    
    FILE *fp = fopen("savedata/scores.txt", "a+");
			time_t  timer = time(NULL);
			fprintf(fp,"\t%15s score = %d\tv1.5\t%s\n\n",player_name,tower_level*10+player_status[0]*5+kill_num*2+dust/100,ctime(&timer));
    		fclose(fp);
    
    system("pause");
    
    
	return 0;
}

void player_creat(){
	
	int point=1;
	
	char choose[2]={'0'};
	
	printf("請輸入玩家名稱(Max 15 words):");
	
	scanf("%s",&player_name);
	
	while(point>0){
	
	printf("請選擇天賦( %dleft):\n(1)怪力 (你天生神力，開場即會使用重斬)\n(2)鷹眼 (你目光如炬，可以輕易看破怪物的狀態)\n(3)輕靈 (你身輕如燕，躲過怪物的眼睛輕而易舉)\n(4)技能78\n(5)火之子 (你是火的孩子！開場即會使用火球術)\n(6)赫爾魯斯之血 (你繼承了傳說中的血統，當你受傷時會逐漸回復)\n",point);
	
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
			skill[12].learned=true;
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
		default:printf("沒有這個選項!\n");goto unability;
	}
	
	point--;
				   }
				
	point=3;
	
	while(point!=0){
		
	system("cls");
	
	printf("請選擇你的成長方向(%dleft):\n",point);
	
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
		default:printf("沒有這個選項!!\n");goto  unstatus;
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
		if(n==0){
		printf("  ----    ----    ----  \t頭部裝備： %s\n",equ_weared[0].name);	
		}
		else{
		printf("  ----    ----    ----  \t身體裝備： %s\n",equ_weared[1].name);	
		}
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
	if(n==0){
	printf("  ----    ----    ----  \t手部裝備： %s \n",equ_weared[2].name);	
	}
	else{
	printf("  ----    ----    ----  \t腳部裝備： %s \n",equ_weared[3].name);	
	}
	
	
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
	
	
	printf("  ----    ----    ----  \t武    器： %s \n",equ_weared[4].name);	
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
		printf("======================================================================================================================\n");
		printf(" %s LV.%d  HP=%d/%d MP=%d/%d Atk=%d Def=%d MAtk=%d MRes=%d Crit=%d EXP=%d/%d Range=%d Speed=%d  第%d層  %d Dust\n",player_name,player_status[0],player_status[1],player_status[10],player_status[2],player_status[11],player_status[3],player_status[4],player_status[5],player_status[6],player_status[7],player_status[9],player_status[0]+4,player_status[12],player_status[13],tower_level+1,dust);
		printf("======================================================================================================================\n");
	}

	void command_input(){
		
		int n=0;
		
		char input[2]={'0'};
		
		invalid_command:printf("要做什麼?\n");
		
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
				printf("這裡沒有樓梯！\n");
				goto invalid_command;
			}
				
				break;
			}
			case 'o':{
				if(tower[tower_level][player_position-1]==4){
					int trea=rand()%37+2;
					printf("你找到了 %s ！\n",equipments[trea].name);
					equipments[trea].have=1;
					equ_in_bag[max_item]=equipments[trea];
					max_item++;
					tower[tower_level][player_position-1]=-1;
					system("pause");
				}
				else{
					printf("這裡沒有東西可以打開！\n");
					goto invalid_command;
				}
				break;
			}
			case 'i':{
				int item_print=0;
				printf("背包欄位： %d / 7\n",max_item);
				while(item_print<max_item){
					printf("(%d) %s (A= %d MA= %d Def= %d MRes= %d Range= %d)\n",item_print+1,equ_in_bag[item_print].name,equ_in_bag[item_print].atk,equ_in_bag[item_print].matk,equ_in_bag[item_print].P_Def,equ_in_bag[item_print].M_Def,equ_in_bag[item_print].distance);
					item_print++;
				}
				item_command();
				
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
		
		printf("寶物庫！你的辛苦總算有了代價！\n在房間的角落有一個沒開過的寶箱\n");
		
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
				sscanf(info,"%d,%d,%d,%d,%d,%d,%d,%d,%[^\n]",&monster[i-2].ID,&monster[i-2].HP,&monster[i-2].ATK,&monster[i-2].P_def,&monster[i-2].M_def,&monster[i-2].attack_type,&monster[i-2].attack_distance,&monster[i-2].speed,&monster[i-2].name);  
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
void battle_map(){
	int i=0;
	printf("------------------------\n");
	while(i+battle_distance!=9){
		printf(" ")	;
		i++;
	}
	printf("O ");
	i=0;
	while(i<battle_distance){
		printf("- ");
		i++;
	}
	printf("X\n");
	printf("------------------------\n");
}

void battle(){

	map_print();
	status_print();
	int player_speed=player_status[13],monster_speed=nowmonster1.speed;
	int i=2,a=0,random,n=0,compare,turn=1;
	static int which_skills[15];	
	pres_monster1=nowmonster1;
	
	if(player_ability[1]==true){
		
		printf(" %s (HP=%d) 出現了\n",nowmonster1.name,nowmonster1.HP);
	}
	else{
		printf(" %s 出現了！\n",nowmonster1.name);
	}
	i=0;
	
	int damage,id,count=0;
	
	char command[1];
	
	if(nowmonster1.HP<=0 || player_status[1]<=0){battling=false;}
	
	else{
		
		battle_end:random=0;
		
		while(i==0){
		
			if(player_speed>=monster_speed && player_speed>=100){
			battle_map();printf("============\n  TURN %d\n============\n",turn);invalid_command: printf("你的回合！要怎麼做？\n(k)攻擊\n(s)技能選單\n(w)原地等待\n(r)逃跑\n(4)拉開距離\n(6)拉近距離\n");
		
		 scanf("%s",&command[0]);
			
			if(command[0]=='k'){
			if(player_status[12]>=battle_distance){
				system("cls");map_print();
				status_print();
				printf("============\n  TURN %d\n============\n",turn);
				printf("攻擊！\n");
				damage=player_damage_count();	
				nowmonster1.HP=nowmonster1.HP-damage;
				printf("你對怪物造成 %d 點傷害！\n",damage);
			}
			else{
				system("cls");map_print();
				status_print();battle_map();
				printf("============\n  TURN %d\n============\n",turn);
				printf("你跟怪物離得太遠了！\n");
				goto invalid_command;
			}
			}
			else if(command[0]=='w'){
				system("cls");map_print();
				status_print();
				printf("============\n  TURN %d\n============\n",turn);
				printf("你在原地等待！\n");
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
				printf("用哪個技能？\n");
				
				id=1,count=0;
				
				while(id<13){
					
				if(skill[id].learned==true){
						
					printf("(%d) %s───  %s (消費MP=%d) (射程=%d)\n",count+1,skill[id+1].name,skill[id+1].description,skill[id+1].MP,skill[id+1].distance);
					
					which_skills[count]=id+1;
					count++;
											}
					id++;	
							}
				printf("(b)back\n");
				
				invalid_skill_command: scanf("%s",&command[0]);	
				
				if(command[0]=='b'){
				system("cls");map_print();
				status_print();battle_map();
				printf("============\n  TURN %d\n============\n",turn);
					goto invalid_command;
					
				}
				else if(count!=0){
				compare=atoi(command);
				
				if(compare<=count && player_status[2]>=skill[which_skills[compare-1]].MP){
					system("cls");map_print();
				status_print();printf("============\n  TURN %d\n============\n",turn);
				
				if(skill[which_skills[compare-1]].distance>=battle_distance){
				player_status[2]=player_status[2]-skill[which_skills[compare-1]].MP;
				
				skill_call[(skill[which_skills[compare-1]].ID)-1]();	
				}
				else{
					printf("不在有效射程裡！\n");
					goto invalid_command;
				}
	
				

					
				count=0,compare=0;
				}
				else if(compare<=count && player_status[2]<skill[which_skills[compare-1]].MP){
					
					printf("你沒有足夠的魔力！\n'");
					goto invalid_command;
				}
				else{
					printf("無效指令！\n");
					goto invalid_command;
				}
				}
					
				}
				else if(command[0]=='6'){
					if(battle_distance>1){
						battle_distance--;
				system("cls");map_print();
				status_print();
				printf("============\n  TURN %d\n============\n",turn);
						printf("你拉近了跟怪物的距離\n");
					}
					else{
						printf("你跟怪物已經離的夠近了！\n");
						goto invalid_command;
					}
				}
				else if(command[0]=='4'){
					if(battle_distance<5){
						battle_distance++;
				system("cls");map_print();
				status_print();
				printf("============\n  TURN %d\n============\n",turn);
						printf("你跟怪物拉開了距離\n");
					}
					else{
						printf("你跟怪物已經離的夠遠了\n");
						goto invalid_command;
					}
				}
				else{printf("無效指令！\n");goto invalid_command;
				}
				player_speed-=100;	
				
		}else if(monster_speed>player_speed && monster_speed>=100 && nowmonster1.HP >0){
			monster_action();
			monster_speed-=100;
			if(debuff_poison_count!=0){
			int poison_damage=debuff_poison_count*5*pres_monster1.HP/100;
			nowmonster1.HP-=poison_damage;
			printf("怪物中毒了！受到 %d 點傷害！\n",poison_damage);		}	
			turn++;	
		}
		else{
			player_speed+=player_status[13];
			monster_speed+=nowmonster1.speed;
		}
		if(nowmonster1.HP<=0){
			printf("你打敗了怪物！牠化成一堆塵之後消失了\n");
			tower_monster_living[tower_level][player_position-1]=false;
			battling=false;
			player_status[9]++;
			
			int gold=(tower_level+1)*(rand()%5+5);
			dust=dust+gold;
			
			printf("你得到了 %d 塵.\n",gold);
			i++;
			debuff_poison_count=0;
			debuff_atk_count=0;
			buff_def_count=0;
			kill_num++;
		}
		if(player_status[1]<=0){
		i++;}
	
		}
		}
		if(player_status[2]!=player_status[11]){
			player_status[2]+=player_status[11]*10/100;
			if(player_status[2]>player_status[11]){
				player_status[2]=player_status[11];
			}
		}
		if(player_ability[0]==true&&player_status[1]!=player_status[10]){
			player_status[1]+=player_status[10]*1/100;
			if(player_status[1]>player_status[10]){
				player_status[1]=player_status[10];
			}
			printf("你的血在沸騰！你感覺自己的體力稍稍回復了！\n");
		}	battle_distance=3;
		}
	
	void monster_action(){
		if(nowmonster1.attack_distance>=battle_distance){
			
			int monster_choose=rand()%100+1;
			
			if(nowmonster1.attack_distance>player_status[12] && monster_choose<=30*(nowmonster1.attack_distance-player_status[12])){
				battle_distance++;
				printf("怪物與你拉開了距離！\n");
			}
			else{
			if(player_ability[1]==true){
				printf("%s (HP=%d) 攻擊你！\n",nowmonster1.name,nowmonster1.HP);
			}
			else{
			printf("%s 攻擊你！\n",nowmonster1.name);	
			}
		
			int damage=monster_damage_count();
			
			printf("怪物對你造成了 %d 點傷害！\n",damage);
			
			player_status[1]=player_status[1]-damage;		
			}
		}
		else{
			battle_distance--;
			printf("怪物衝向了你！\n");
		}	
			buff_print();
			status_print();

	}
		
	
			
		
	
	


	int player_damage_count(){
		
		int i=rand()%25+85,cri=rand()%100+1;
		
		i=(player_status[3]*i-nowmonster1.P_def)/100;
		
		if(cri<=player_status[7]){
			
			i=i*2;
			printf("爆擊！\n");
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
	
	
	printf("你升級了！\n請選擇你的成長方向！(%d left):\n",point);
	
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
		default:printf("無此選項！\n");goto  unstatus;
	}
	
	point--;
		
				   }
	system("cls");
	
	if(player_status[0]%5==0){
		
		int random=rand()%11+2,which_skill[3],choose_what;
		
		printf("請選擇一項想學習的技能！\n");
		int i=0;
		while(i<3){
			if(skill[random].learned==false){
			printf("(%d)%s─── %s (MP消耗=%d) (射程=%d)\n",i+1,skill[random].name,skill[random].description,skill[random].MP,skill[random].distance);
			which_skill[i]=skill[random].ID;
			i++;
			}
		random=rand()%11+2;
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
			printf("無此選項！\n");
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
            while(fgets(temp, sizeof(temp), fp) && i<16)  //當讀到第random行時，要指定怪的話就改成ID+1(跳過標題標示) 
            {
				sscanf(temp,"%d,%d,%d,%[^,],%d,%[^\n]",&skill[i-1].ID,&skill[i-1].type,&skill[i-1].MP,skill[i-1].description,&skill[i-1].distance,skill[i-1].name);  
                if(skill[i-2].distance==0){
                	skill[i-2].distance=player_status[12];
				}
				i++;
			}
			a++;
        }
        fclose(fp);
	}
void equipments_load(){
	int i=1,a=0;
	char temp[250];
	FILE *fp;
       if( ( fp = fopen( "data/equipments.csv", "r")) == NULL) //沒找到檔案就結束 
       {
              puts("Cannot open the file");
       }
       while (a==0) //開始一行一行往下，從檔案讀進info字串 
		{
            while(fgets(temp, sizeof(temp), fp) && i<42)  //當讀到第random行時，要指定怪的話就改成ID+1(跳過標題標示) 
            {
				sscanf(temp,"%d,%d,%d,%d,%d,%d,%d,%d,%[^\n]",&equipments[i-1].ID,&equipments[i-1].P_Def,&equipments[i-1].M_Def,&equipments[i-1].atk,&equipments[i-1].matk,&equipments[i-1].type,&equipments[i-1].distance,&equipments[i-1].price,equipments[i-1].name);  
				i++;
			}
			a++;
        }
     
        fclose(fp);
	}
void equipment_display(void){
	int i;
	int num;
   	int counter=1;
   	for(i=2;i<amount_of_equipments;i++) 
   		if(equipments[i].have==1){
   			 printf("%2d.%-30s physical DEF:%-3d magical DEF:%-3d HP:%-3d MP:%d\n",counter,equipments[i].name,equipments[i].P_Def,equipments[i].M_Def,equipments[i].atk,equipments[i].matk);
   			 counter++;
	}
}
void buff_print(){
	
	if(debuff_atk_count!=0){
				debuff_atk_count--;
				if(debuff_atk_count==0){
		nowmonster1.ATK=pres_monster1.ATK;
		printf("怪物的戰鬥力回復了！\n");
								}		
				else{
		printf("怪物攻擊的力道變弱了！還剩 %d 回合\n",debuff_atk_count);
					}
			}
	if(buff_def_count!=0){
				buff_def_count--;
				if(buff_def_count==0){
					player_status[4]=pres_player_status[4];
					printf("你的身體能力回復了，小心！\n");
	}
	else{
		printf("你變得比以前更堅韌，還剩 %d 回合！\n",buff_def_count);
	}
			}
	
	
	
	
	
}
	
void monster_command(){
	nowmonster1=monster[tower_monster[tower_level][player_position-1]];
	
	if(player_ability[1]==true){
		printf("你看到 %s (HP=%d) 在房間裡，要怎麼做？\n(1)戰鬥\n(2)潛行\n(3)返回\n",nowmonster1.name,nowmonster1.HP);
	}
	else{
		printf("你看到 %s 在房間裡，要怎麼做？\n(1)戰鬥\n(2)潛行\n(3)返回\n",nowmonster1.name);
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
				printf("怪物似乎沒有注意到你！但你若繼續留在這裡很有可能會被發現......\n");
			}
			else{
				printf("失敗！怪物向你突襲！\n");
				int damages=monster_damage_count();
				printf("怪物對你造成了 %d 點傷害！\n",damages);
				player_status[1]-=damages;
				system("pause");
				system("cls");
				if(player_status[1]!=0){
				battling=true;
				battle();	
				}
				
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
		default:printf("無效指令！");goto invalid_monster_command;

	}
	
}
	void item_command(){
		char input[2]={'0'};
		invalid_command: printf("要做什麼？\n");
		scanf("%s",&input[0]);
		int choose=atoi(input);
		if(choose<=max_item){
				player_status[3]-=equ_weared[equ_in_bag[choose-1].type-1].atk;
				player_status[5]-=equ_weared[equ_in_bag[choose-1].type-1].matk;
				player_status[4]-=equ_weared[equ_in_bag[choose-1].type-1].P_Def;
				player_status[6]-=equ_weared[equ_in_bag[choose-1].type-1].M_Def;
			
			equ_weared[equ_in_bag[choose-1].type-1]=equ_in_bag[choose-1];
			printf("你裝備了 %s ！\n",equ_in_bag[choose-1].name);
				player_status[3]+=equ_weared[equ_in_bag[choose-1].type-1].atk;
				player_status[5]+=equ_weared[equ_in_bag[choose-1].type-1].matk;
				player_status[4]+=equ_weared[equ_in_bag[choose-1].type-1].P_Def;
				player_status[6]+=equ_weared[equ_in_bag[choose-1].type-1].M_Def;
			if(equ_in_bag[choose-1].type==5){
				player_status[12]=equ_weared[4].distance;
			}
			system("pause");
		}
		else{
			printf("你沒有這個物品！\n");
			goto invalid_command;
			
		}
	}
