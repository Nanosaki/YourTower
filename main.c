#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <memory.h>
#define max_length_of_name 50

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

static	int tower[100][9],tower_road[100][9],tower_monster[100][9],tower_situa[100][9];//tower[層數][房間編號]存房間類型  tower_road存通道 player_ability存天賦 
	
static	int player_status[15]={1,100,50,30,10,20,10,7,50,0,100,50,2,100},pres_player_status[15]={1,100,50,30,10,20,10,7,50,0,100,50},skillnum=0,dust=0;//player_Status={0Level,1HP,2MP,3Atk,4def,5Matk,6Mdef,7Critical,8Rstealth,9EXP
						//				10MHP,11MMP,12range,13speed}
static int buff_def_count=0,debuff_atk_count=0,debuff_poison_count=0,battle_distance=3,amount_of_equipments=40,tt=0;

static	char player_name[15];

static int player_position=8,pres_player_position=8,tower_level=0,kill_num=0,max_item=7,equip_num=0,item_num=0,item_created=0;

static bool battling=false,just_up=false,player_ability[6],tower_monster_living[100][9],in_town=true;

void leader_of_village(void); 
void teleporter(void);
void seller_of_potion(void);
void seller_of_equipment(void);
struct itemcreate  //建立結構 
{
	int A1,MA1,D1,MR1,A2,MA2,D2,MR2,A3,MA3,D3,MR3,type,distance,speed;
	char material[100],set[100],armor[100];
	bool obtained;
}itemcreate[50];
struct item{
	int type; //
	char name[max_length_of_name]; //道具名稱 
	char description[max_length_of_name]; 
	int ID; //道具ID 
	int price;
	int amount; // 
	bool have; //是否擁有 
	bool used_in_battle;  
}items[40],item_inbag[10];
struct armor{
	int P_Def,distance,speed; //物防 
	int M_Def; //魔防
	int atk; //血量 
	int matk,type; //魔力 
	char name[max_length_of_name]; //裝備名稱 
	int ID; //裝備ID 
	int price;
	bool have; //是否擁有裝備 
	bool is_weared; //是否穿戴裝備 
}equipments[60],equ_in_bag[10],equ_weared[5];

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
		int i=rand()%20+200,cri=rand()%100+1;
		
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
		
		int i=rand()%15+135;
		
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
		
		int i=rand()%30+120;
		
		i=player_status[3]*i/100;
		
		nowmonster1.HP-=i;
		
		printf("使用 %s !!怪物在無法抵抗的狀況下受到 %d 點傷害!\n",skill[4].name,i);
	}
	void skill4(){
		
		int i=rand()%5+2;
		
		i=dust/20*i;
		
		nowmonster1.HP-=i;
		
		printf("你引爆了 %d 份的塵並造成 %d 點傷害，多麼奢侈!\n",dust/100,i);
		
		dust=dust-dust/100;
	}
	void skill5(){
		
		int i=rand()%100+1;
		
		if(i<=9){
			
			nowmonster1.HP=0;
			
			printf("你將怪物切成兩半!\n");
			
		}
		else{
			printf("失敗，怪物毫無破綻!\n");
		}
			
	}
	void skill6(){
		
		int i=rand()%25+85,cri=rand()%100-54;
		
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
   

    void item1()//香蕉<小> 
	{ 
		int i=player_status[10]*0.15;
		
	
		if(i>player_status[10]-player_status[1]){
			i=player_status[10]-player_status[1];
		}
		player_status[1]+=i;
		
		printf("使用 %s !!你回復了 %d 點血量!\n",items[1].name,i);
	}
	void item2()//香蕉<中>
	{ 
		int i=player_status[10]*0.25; 
		
		if(i>player_status[10]-player_status[1]){
			i=player_status[10]-player_status[1];
		}
		player_status[1]+=i;
		
		printf("使用 %s !!你回復了 %d 點血量!\n",items[2].name,i);
		
		
	}
	void item3()//香蕉<大>
	{	int i=player_status[10]*0.5;
		
		if(i>player_status[10]-player_status[1]){
			i=player_status[10]-player_status[1];
		}
		player_status[1]+=i;
		
		printf("使用 %s !!你回復了 %d 點血量!\n",items[3].name,i);
	}
	void item4()//海水<小> 
	{
		int i=player_status[11]*0.2;
		
		if(i>player_status[11]-player_status[2]){
			i=player_status[11]-player_status[2];
		}
		player_status[2]+=i;
		
		printf("使用 %s !!你回復了 %d 點魔力!\n",items[4].name,i);
	}
    void item5()//海水<中>
	{
		int i=player_status[11]*0.4;
		if(i>player_status[11]-player_status[2]){
			i=player_status[11]-player_status[2];
		}
		player_status[2]+=i;
		
		printf("使用 %s !!你回復了 %d 點魔力!\n",items[5].name,i);
		}
	void item6()//海水<大>
	{
		int i=player_status[11]*0.6;
		if(i>player_status[11]-player_status[2]){
			i=player_status[11]-player_status[2];
		}
		player_status[2]+=i;
		
		printf("使用 %s !!你回復了 %d 點魔力!\n",items[6].name,i);
	}
	void item7()//某人的情書 
	{
		
		player_status[1]=player_status[10];
		player_status[2]=player_status[11];
		
		printf("愛情帶給了你力量!血量與魔力完全恢復了!\n");
		
		
	}
	void item8()//飛刀 
	{
		int i=130*player_status[3]/100-nowmonster1.P_def;
		
		if(i<0){
			i=0;
		}
		
		nowmonster1.HP-=i;
		
		printf("丟出 %s !!你對敵人造成 %d 點傷害!\n",items[8].name,i);
	}
	void item9()//手裏劍 
	{
		int i=200*player_status[3]/100-nowmonster1.P_def;
		
		if(i<0){
			i=0;
		}
		
		nowmonster1.HP-=i;
		
		printf("擲出 %s !!你對敵人造成 %d 點傷害!\n",items[9].name,i);
	}
	void item10()//爆破炸彈 
	{
		int i=350*player_status[3]/100;
		
		if(i<0){
			i=0;
		}
		
		nowmonster1.HP-=i;
		
		printf("投擲 %s !!你對敵人造成 %d 點傷害!\n",items[10].name,i);
	}
	void item11()//卷軸<米吉多> 
	{
		int i=130*player_status[5]/100-nowmonster1.M_def;
		
		nowmonster1.HP-=i;
		
		printf("釋放 %s !!你對敵人造成 %d 點傷害!\n",items[11].name,i);
	}
	void item12()//卷軸<米吉多拉> 
	{
		int i=200*player_status[5]/100-nowmonster1.M_def;
		
		nowmonster1.HP-=i;
		
		printf("釋放 %s !!你對敵人造成 %d 點傷害!\n",items[12].name,i);
	}
	void item13()//卷軸<米吉多拉翁> 
	{
		int i=350*player_status[5]/100;
		
		nowmonster1.HP-=i;
		
		printf("釋放 %s !!你對敵人造成 %d 點傷害!\n",items[13].name,i);
	}
	void item14()//卷軸<煙幕> 
	{
					player_position=pres_player_position;
					
					printf("釋放 %s !!你逃走了！\n",items[14].name);
					
					battling=false;

	}
	void item15()//卷軸<傳送> 
	{
		in_town=1;
		printf("釋放 %s !!你離開了迷宮\n",items[15].name);
		system("pause");
	}
	void (*item_call[15])()={item1,item2,item3,item4,item5,item6,item7,item8,item9,item10,item11,item12,item13,item14,item15};	
	
int main(int argc, char *argv[]) {
	
	srand(time(NULL));
	
	material_load();
	
	item_load();
	
	skill_load();
	
	equipments_load();
	
	monster_creat();
	
    tower_creat();
    
    player_creat();
    
    system("cls");
    
	while(player_status[1]>0){
		
		while(in_town==1){
			
			town_print();
			
			town_action();
			
		}
		while(in_town==0){
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
		
	
	}
	system("cls");
    printf("You DIED.\nYour score is : %d\n",tower_level*10+player_status[0]*5+kill_num*2+dust/100);
    
    FILE *fp = fopen("savedata/scores.txt", "a+");
			time_t  timer = time(NULL);
			fprintf(fp,"\t%15s score = %d\tv1.62\t%s\n\n",player_name,tower_level*10+player_status[0]*5+kill_num*2+dust/100,ctime(&timer));
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
	
	printf("請選擇天賦( %dleft):\n(1)怪力 (你天生神力，開場即會使用重斬)\n(2)鷹眼 (你目光如炬，可以輕易看破怪物的狀態)\n(3)輕靈 (你身輕如燕，躲過怪物的眼睛輕而易舉)\n(4)收藏家 (你擅長收納，能比別人攜帶更多物品)\n(5)火之子 (你是火的孩子！開場即會使用火球術)\n(6)赫爾魯斯之血 (你繼承了傳說中的血統，當你受傷時會逐漸回復)\n",point);
	
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
			max_item+=3;
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
		
		tower[i][y]=rand()%5+1;
		
		tower_road[i][y]=(rand()%3)+1;
		
		tower_situa[i][y]=(rand()%4)+1;
		
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
					int trea=rand()%5+1;
					if(trea<3){//裝備生成 
					trea=rand()%29+1;
					itemcreate[0]=itemcreate[trea];
					
					if(itemcreate[0].type==5){
						trea=rand()%20+1;
					}
					else{
						trea=rand()%29+8;
					}
						strcpy(itemcreate[0].material,itemcreate[trea].material);
						itemcreate[0].A1=itemcreate[trea].A1;
						itemcreate[0].D1=itemcreate[trea].D1;
						itemcreate[0].MA1=itemcreate[trea].MA1;
						itemcreate[0].MR1=itemcreate[trea].MR1;
						trea=rand()%29+1;
						strcpy(itemcreate[0].set,itemcreate[trea].set);
						itemcreate[0].A2=itemcreate[trea].A2;
						itemcreate[0].D2=itemcreate[trea].D2;
						itemcreate[0].MA2=itemcreate[trea].MA2;
						itemcreate[0].MR2=itemcreate[trea].MR2;		
						strcpy(equipments[59].name,"");
						strcat(equipments[59].name,itemcreate[0].material);
						strcat(equipments[59].name,itemcreate[0].set);
						strcat(equipments[59].name,itemcreate[0].armor);
						trea=(rand()%4)*itemcreate[0].A1;
						int A2=(rand()%4)*itemcreate[0].A2,A3=(rand()%4)*itemcreate[0].A3;
						equipments[59].atk=trea+A2+A3;
						trea=(rand()%4)*itemcreate[0].MA1;
						A2=(rand()%4)*itemcreate[0].MA2,A3=(rand()%4)*itemcreate[0].MA3;
						equipments[59].matk=trea+A2+A3;
						trea=(rand()%4)*itemcreate[0].D1;
						A2=(rand()%4)*itemcreate[0].D2,A3=(rand()%4)*itemcreate[0].D3;
						equipments[59].P_Def=trea+A2+A3;
						trea=(rand()%4)*itemcreate[0].MR1;
						A2=(rand()%4)*itemcreate[0].MR2,A3=(rand()%4)*itemcreate[0].MR3;
						equipments[59].M_Def=trea+A2+A3;
						equipments[59].type=itemcreate[0].type;
						equipments[59].speed=itemcreate[0].speed;
						equipments[59].distance=itemcreate[0].distance;
						printf("你找到了 %s ！\n",equipments[59].name);
						item_created++;
						equipments[59].ID=item_created+50;
					if(equip_num+item_num<max_item){
					equipments[59].have=1;
					equ_in_bag[equip_num]=equipments[59];
					equip_num++;
					}
					else{
						printf("你的背包滿了！請選擇要丟棄的東西！\n");
						tt=1;
						item_drop(tt);
					}
					}
					else{//道具生成 
					trea=rand()%15+1;
					items[50]=items[trea];
					printf("你找到了 %s ！\n",items[50].name);
					int count=0,bb=0;
						while(count<item_num){
							if(item_inbag[count].ID==items[50].ID){
								item_inbag[count].amount++;bb=1;
							}
							count++;
						}
					if(equip_num+item_num<max_item||bb==1){
						if(bb!=1){
						item_inbag[item_num]=items[50];
						item_inbag[item_num].amount++;
						item_num++;
						}
					}
					else{
						printf("你找到了 %s ！但你的背包已經裝不下了，請選擇要丟棄的物品。\n ",items[50].name);
						tt=2;
						item_drop(tt);
					} 
					}
					
					
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
				printf("背包欄位： %d / %d\n",equip_num+item_num,max_item);
				while(item_print<equip_num){
					printf("(%d) %-15s (A= %-2d  MA= %-2d  Def= %-2d  MRes= %-2d  Range= %d)\n",item_print+1,equ_in_bag[item_print].name,equ_in_bag[item_print].atk,equ_in_bag[item_print].matk,equ_in_bag[item_print].P_Def,equ_in_bag[item_print].M_Def,equ_in_bag[item_print].distance);
					item_print++;
				}
				while(item_print<equip_num+item_num){
					printf("(%d) %-15s %-30s 剩餘 %-2d 個\n",item_print+1,item_inbag[item_print-equip_num].name,item_inbag[item_print-equip_num].description,item_inbag[item_print-equip_num].amount);
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
		
		if(tower_situa[tower_level][player_position-1]==1){
			printf("空氣中飄著陳腐的味道......是書！這裡是書房！\n");
		}
		else if(tower_situa[tower_level][player_position-1]==2){
			printf("到處都是散落的紙本，你撿起一看，歪曲的文字似乎表示著這裡的歷史\n");
		}
		else{
			printf("寒冷的風撲鼻而來，你不禁打了個噴嚏\n");
		}
		
	}else if(tower[tower_level][player_position-1]==3){
		if(tower_situa[tower_level][player_position-1]==1){
			printf("房間的中央矗立著不祥的雕像，你盯著他，感覺自己快要發狂\n");
		}else if(tower_situa[tower_level][player_position-1]==2){
			printf("地上的打鬥痕跡，空氣中瀰漫著金屬的臭味，是血！這裡一定發生過激烈的戰鬥！\n");
		}else{
			printf("不明的壓迫感襲向胸口，這裡的重力似乎比平常還強，連移動一步都要多花力氣\n");
		}
	}else if(tower[tower_level][player_position-1]==4){
		
		printf("寶物庫！你的辛苦總算有了代價！\n在房間的角落有一個沒開過的寶箱\n");
		
	}else if(tower[tower_level][player_position-1]==5){
		if(tower_situa[tower_level][player_position-1]==1){
		printf("很香的味道......這裡應該是廚房。\n你不由自主地偷看了一下大釜中的食物......噁，是人肉！\n");
		}else if(tower_situa[tower_level][player_position-1]==2){
			printf("皮膚感到一陣黏膩，一陣悶熱撲來，你感到如身處雨林般的不適\n");
		}else{
			printf("牆上歪歪扭扭的寫著某些字「獻祭…讚美東…」似乎不去深思比較好\n");
		}
		
		
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
        		
        		tower_monster[i][a]=ceil((rand()%16+1)*(i+1)/24)+1;
        		
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
			battle_map();printf("============\n  TURN %d\n============\n",turn);invalid_command: printf("你的回合！要怎麼做？\n(k)攻擊\n(s)技能選單\n(i)道具選單\n(w)原地等待\n(r)逃跑\n(4)拉開距離\n(6)拉近距離\n");
		
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
						
					printf("(%d) %-15s───  %-30s (消費MP=%-2d) (射程=%d)\n",count+1,skill[id+1].name,skill[id+1].description,skill[id+1].MP,skill[id+1].distance);
					
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
				else if(command[0]=='i'){
					if(item_num==0){
						printf("你沒有任何道具！\n");
						goto invalid_command;
					}
					else{
					printf("用哪個道具？\n");
					int item_print=0;
					while(item_print<item_num){
					printf("(%d) %-15s %-30s 剩餘 %-2d 個\n",item_print+1,item_inbag[item_print].name,item_inbag[item_print].description,item_inbag[item_print].amount);
					item_print++;}
					printf("(b)取消\n");
					scanf("%s",command);
					if(command[0]=='b'){
						goto invalid_command;
					}
					int item_choose=atoi(command);
						if(items[item_inbag[item_choose-1].ID-1].type!=3&&item_inbag[item_choose-1].amount>0){
							system("cls");map_print();
				status_print();
				printf("============\n  TURN %d\n============\n",turn);
							item_call[(item_inbag[item_choose-1].ID)-1]();
							item_inbag[item_choose-1].amount-=1;
							if(battling==false){
								break;
							}
						}
						else if(items[item_inbag[item_choose-1].ID-1].type!=3&&item_inbag[item_choose-1].amount<1){
							system("cls");map_print();
				status_print();
				printf("============\n  TURN %d\n============\n",turn);
							printf("你沒有這個道具了！\n");
							goto invalid_command;
						}
						else{
							system("cls");map_print();
				status_print();
				printf("============\n  TURN %d\n============\n",turn);
							printf("這個道具不能在戰鬥中使用！\n");
							goto invalid_command;
						}	
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
			printf("(%d)%-15s─── %-30s (MP消耗=%-2d) (射程=%d)\n",i+1,skill[random].name,skill[random].description,skill[random].MP,skill[random].distance);
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
            while(fgets(temp, sizeof(temp), fp) && i<38)  //當讀到第random行時，要指定怪的話就改成ID+1(跳過標題標示) 
            {
				sscanf(temp,"%d,%d,%d,%d,%d,%d,%d,%d,%[^\n]",&equipments[i-1].ID,&equipments[i-1].P_Def,&equipments[i-1].M_Def,&equipments[i-1].atk,&equipments[i-1].matk,&equipments[i-1].type,&equipments[i-1].distance,&equipments[i-1].price,equipments[i-1].name);  
				i++;
			}
			a++;
        }
        fclose(fp);
        
	}  
void item_load(){
	int i=1,a=0;
	char temp[250];
		
	FILE *fp;
		if( ( fp = fopen( "data/items.csv", "r")) == NULL) //沒找到檔案就結束 
       {
              puts("Cannot open the file");
       }
       while (a==0) //開始一行一行往下，從檔案讀進info字串 
		{
            while(fgets(temp, sizeof(temp), fp) && i<17)  //當讀到第random行時，要指定怪的話就改成ID+1(跳過標題標示) 
            {
				sscanf(temp,"%d,%d,%[^,\n],%[^,\n],%d",&items[i-1].ID,&items[i-1].type,items[i-1].name,items[i-1].description,&items[i-1].price);  
				i++;
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
	nowmonster1.HP+=nowmonster1.HP*tower_level/20;
	nowmonster1.ATK+=nowmonster1.ATK*tower_level/50;
	nowmonster1.P_def+=nowmonster1.P_def*tower_level/35;
	nowmonster1.M_def+=nowmonster1.M_def*tower_level/35;
	
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
		invalid_command: printf("(0)取消\n要做什麼？\n");
		scanf("%s",&input[0]);
		int choose=atoi(input);
		if(choose==0){
			printf("你闔上了背包\n");
			system("pause");
		}
		else if(choose<=equip_num){
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
		else if(choose>equip_num&&choose<=equip_num+item_num){
			if(item_inbag[choose-equip_num-1].type!=2 &&item_inbag[choose-equip_num-1].amount>0){
				item_call[(item_inbag[choose-equip_num-1].ID)-1]();
				item_inbag[choose-equip_num-1].amount-=1;
				system("pause");
			}
			else if(item_inbag[choose-equip_num-1].type!=2 &&item_inbag[choose-equip_num-1].amount>0){
				printf("你這個道具已經用完了！\n");
				goto invalid_command;
			}
			else{
				printf("你不能在非戰鬥中使用這個物品！\n");
				goto invalid_command;
			}
		}
		else{
			printf("你沒有這個物品！\n");
			goto invalid_command;
			
		}
	}
	
void material_load(){
		int i=2,a=0;
	char temp[100];
	FILE *fp;

       if( ( fp = fopen( "data/material.csv", "r")) == NULL) //沒找到檔案就結束 
       {
              puts("Cannot open the file");
       }
       
       while (a==0) //開始一行一行往下，從檔案讀進info字串 
		{
            
            while(fgets(temp, sizeof(temp), fp) && i<32)  //當讀到第random行時，要指定怪的話就改成ID+1(跳過標題標示) 
            {
				sscanf(temp,"%[^,],%d,%d,%d,%d,%[^,],%d,%d,%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%d",itemcreate[i-2].material,&itemcreate[i-2].A1,&itemcreate[i-2].MA1,&itemcreate[i-2].D1,&itemcreate[i-2].MR1,itemcreate[i-2].set,&itemcreate[i-2].A2,&itemcreate[i-2].MA2,&itemcreate[i-2].D2,&itemcreate[i-2].MR2,itemcreate[i-2].armor,&itemcreate[i-2].A3,&itemcreate[i-2].MA3,&itemcreate[i-2].D3,&itemcreate[i-2].MR3,&itemcreate[i-2].type,&itemcreate[i-2].distance,&itemcreate[i-2].speed);  
                i++;
                
				//使用isscanf將info字串內的東西格式化(類似分割)，並依序存入結構 
			}
			a++;
        }
        fclose(fp);
	}
void item_drop(){
				int item_print=0,choose=0;
				printf("(0)");
				if(tt==1){
				printf("%-15s (A= %-2d MA= %-2d Def= %-2d MRes= %-2d Range= %d)\n",equipments[59].name,equipments[59].atk,equipments[59].matk,equipments[59].P_Def,equipments[59].M_Def,equipments[59].distance);	
				}
				else{
				printf("%-15s %-30s \n",items[50].name,items[50].description);	
				}
				
				while(item_print<equip_num){
					printf("(%d) %-15s (A= %-2d MA= %-2d Def= %-2d MRes= %-2d Range= %-d)\n",item_print+1,equ_in_bag[item_print].name,equ_in_bag[item_print].atk,equ_in_bag[item_print].matk,equ_in_bag[item_print].P_Def,equ_in_bag[item_print].M_Def,equ_in_bag[item_print].distance);
					item_print++;
				}
				while(item_print<equip_num+item_num){
					printf("(%d) %-15s %-30s 剩餘 %-2d 個\n",item_print+1,item_inbag[item_print-equip_num].name,item_inbag[item_print-equip_num].description,item_inbag[item_print-equip_num].amount);
					item_print++;
				}
				invalid_command: scanf("%d",&choose);
				
				if(choose==0){
					printf("你放棄了撿取\n");
				}
				else if(choose<=equip_num+item_num){
					int count=0;
					while(count<6){
						if(equ_weared[count].ID==equ_in_bag[choose-1].ID){
				player_status[3]-=equ_weared[count].atk;
				player_status[5]-=equ_weared[count].matk;
				player_status[4]-=equ_weared[count].P_Def;
				player_status[6]-=equ_weared[count].M_Def;
							equ_weared[count]=equipments[58];
						}
						count++;
					}
					if(choose<=equip_num&&tt==1){
						equ_in_bag[choose-1]=equipments[59];
					}
					else if(choose<=equip_num&&tt==2){
						int i=choose-1;
						while(i<equip_num){
							equ_in_bag[i]=equ_in_bag[i+1];
							i++;
						}
						item_inbag[item_num]=items[50];
						item_inbag[item_num].amount++;
						item_num++;
						equip_num--;	
					}
					else if(choose>equip_num&&tt==1){
						equ_in_bag[equip_num]=equipments[59];
						int i=choose-equip_num-1;
						while(i<item_num){
							item_inbag[i]=item_inbag[i+1];
							i++;
						}
						item_num--;
						equip_num++;
					}
					else{
						item_inbag[choose-equip_num-1]=items[50];
						item_inbag[choose-equip_num-1].amount++;
					}
				}
				else{
					printf("無效指令！\n");
					goto invalid_command;
				}
}

void seller_of_potion(){

	int i,j;
	int t=0;
	for(i=1;i<15;i++)  // 印出商店道具 
	printf("%-2d:%-20s %-40s 價錢:%d \n",i,items[i].name,items[i].description,items[i].price);
	printf("請盡情挑選!!\t\t\t\t\t\t\t\t持有塵： %d\n輸入你想要的道具的數字(0離開)\n",dust);
		while(t==0){
		invalid_shop: scanf("%d",&j);
		for(i=1;i<15;i++){int bb=0;
			if(j==items[i].ID&&equip_num+item_num<=max_item&&dust>=items[i].price){
				int k;
				for(k=0;k<item_num;k++){
					if(j==item_inbag[k].ID){
						item_inbag[k].amount++;bb=1;
					}
				}
				if(bb!=0){
					item_inbag[item_num]=items[k];
				}
			dust-=items[i].price;
			 printf("你購買了 %s.\n",items[i].name);
		}
			else if(j==items[i].ID&&equip_num+item_num>max_item){
				items[50]=items[i];
				dust-=items[i].price;
				tt=2;
				item_drop();
			}
			else if(j>14){
				printf("沒有這個商品！\n");
				goto invalid_shop;
			}
			else if(j==0){
			 t=1;
					}
			else{
				printf("你沒有足夠的塵！\n");
				break;
			}
}
}
printf("歡迎常來!!\n");
}
void seller_of_equipment(){int i = 0, j = 0;
		int temp1 = 0;
		int temp2 = 0;
		int temp3 = 0;
		int temp4 = 0;
		int temp5 = 0;
		int temp6 = 0;
		int temp9 = 0;
		char temp7[100];
		char temp8[100];
		int t=0;
	/*for(i=0;i<amount_of_equipments-1;i++)
		printf("%d: %s physical defence:%d Magical defence:%d HP:%d MP:%d Price:%d\n",i+1,equipments[i+2].name,equipments[i+2].P_Def,equipments[i+2].M_Def,equipments[i+2].HP,equipments[i+2].MP,equipments[i+2].price);
	 	printf("Please choose what you want:(Enter the number of what you want to buy(enter 0 to exit))\n");*/ 
    for( i = -1; i < 35; i++) { //依照價錢排序 
        for( j = i; j < 35; j++) {
            if( equipments[j+2].price < equipments[i+2].price ) {
                temp1 = equipments[j+2].price;
                equipments[j+2].price = equipments[i+2].price;
                equipments[i+2].price = temp1;
				
				strcpy(temp7, equipments[j+2].name); 
				strcpy(equipments[j+2].name, equipments[i+2].name); 
				strcpy(equipments[i+2].name, temp7); 
                
                temp2 = equipments[j+2].P_Def;
                equipments[j+2].P_Def = equipments[i+2].P_Def;
                equipments[i+2].P_Def = temp2;
                
                temp3 = equipments[j+2].M_Def;
                equipments[j+2].M_Def = equipments[i+2].M_Def;
                equipments[i+2].M_Def = temp3;
            	
            	temp4 = equipments[j+2].distance;
                equipments[j+2].distance = equipments[i+2].distance;
                equipments[i+2].distance= temp4;
                
                temp5 = equipments[j+2].type;
                equipments[j+2].type = equipments[i+2].type;
                equipments[i+2].type = temp5;
                
                temp6 = equipments[j+2].atk;
                equipments[j+2].atk = equipments[i+2].atk;
                equipments[i+2].atk = temp6;
                
                temp9 = equipments[j+2].matk;
                equipments[j+2].matk = equipments[i+2].matk;
                equipments[i+2].matk = temp9;
            }
        }
    }
for(i=1;i<37;i++)

		printf("%-2d: %-20s A=%2d   MA=%2d   Def=%2d   MRes=%2d   Price:%d\n",i,equipments[i].name,equipments[i].atk,equipments[i].matk,equipments[i].P_Def,equipments[i].M_Def,equipments[i].price);
	while(t==0){printf("請盡情挑選!!\t\t\t\t\t\t\t\t持有塵： %d\n輸入你想要的裝備的數字(0離開)\n",dust);
	invalid_shop:	scanf("%d",&j);
		for(i=0;i<38;i++){
			if(j==equipments[i].ID&&equip_num+item_num<=max_item&&dust>=equipments[i].price){
			
			dust-=items[i].price;
			 printf("你購買了 %s.\n",items[i].name);
			 equ_in_bag[equip_num]=equipments[j];
			 equip_num++;
		}
			else if(j==equipments[i].ID&&equip_num+item_num>max_item){
				equipments[59]=equipments[j];
				dust-=equipments[i].price;
				tt=1;
				item_drop();
			}
			else if(j>36){
				printf("沒有這個商品！\n");
				goto invalid_shop;
			}
			else if(j==0){
			 t=1;
					}
			else{
				printf("你沒有足夠的塵！\n");
				break;
			}
	}
}
printf("hope to see you soon.\n");
}

void town_print(){
	system("cls");
	status_print();
	printf("你人在塔外的一座村莊裡，村莊雖然荒涼，但至少你還是能夠進行補給。\n");
	printf("在村莊裡，你能夠看到的人有這些......\n(1)道具商人\n(2)裝備商人\n(3)村長\n(4)離開村莊，冒險\n");
}


void town_action(){
	int choose=0;
	invalid_command: scanf("%d",&choose);
	
	if(choose==1){
		system("cls");
		seller_of_potion();
	}
	else if(choose==2){
		system("cls");
		seller_of_equipment();
		
	}
	else if(choose==3){
		leader_of_village();
	}
	else if(choose==4){
		system("cls");
		printf("你進入了塔中......\n");
		system("pause");
		in_town=0; 
		system("cls");
	}
	else{
		printf("無效指令\n");
		goto invalid_command;
	}
}

void leader_of_village(void){  
	static int leader_of_village_open=0; //村長開場白 只跑一次的判定
	if(leader_of_village_open<1){ //讓開場白只跑一次 
		printf("村長：歡迎來到村莊！我是這裡的村長。");
		leader_of_village_open=1;
	}
	else{
	int a;
	a=(rand() % 5) +1;
	if(a==1){
		printf("村長：你好啊，朋友。\n");
	}
	else if(a==2){
		printf("村長：準備去冒險了嗎？\n");
	}
	else if(a==3){
		printf("村長：可以不要只玩五分鐘嗎。\n");
	}
	else if(a==4){
		printf("村長：雖然我已經六十多歲，但我至今，仍是處男。\n");
	}
	else if(a==5){
		printf("村長：上任當村長，得先巧立名目，拉攏豪紳，繳稅捐款...\n");
	}
	}
	system("pause");	
}
