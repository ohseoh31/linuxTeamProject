#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <gtk/gtk.h>
    
#define BUF_SIZE 100
#define NAME_SIZE 20
    
void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);
  
char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];
char id[5];
int num=0;
int sock;

/*
다른 Widget을 포함할 수 있도록 하는 것중  가장 기본적인 것이 GtkBin이다 GtkBin도 GtkContainer를 상속받아 
Widget이 하나의 다른 Widget만을 포함할 수 있도록 하는 추상적인 Class이다. 
여기서 중요한것은 오직 하나만 포함한다는것이다. 
GtkWindow가 GtkBin을 상속받아 다른 Widget을 포함하는 대표적인 예이다. 
GtkWindow에는 gtk_container_add 함수를 호출하여 오직 하나의 다른 Widget만을 포함하게 할 수 있다.
*/

GtkWidget *frame; //GtkFixed를 Global로 선언한다.
		//Callback 함수에서 여러개의 Widget을 사용할 일이 있어서이다.

//시그널종료 콜백함수---------------------------------------------------------------------------------------------
void quit(GtkWidget *window, gpointer data){
    gtk_main_quit();
}

//고객목록 출력 함수----------------------------------------------------------------------------------------------
void customer_register(GtkWidget *widget, gpointer data){
    write(sock,"01", 2);
    //위젯 선언----------------------------------------------------------------------
    GtkWidget *dialog;
    GtkWidget *name_label, *number_label, *phone_label, *address_label, *detail_label;
    GtkWidget *name_entry, *number_entry, *phone_entry, *address_entry, *detail_entry;
    GtkWidget *name_hbox, *number_hbox, *phone_hbox, *address_hbox, *detail_hbox;
    GtkWidget *message_label;
    
    gint result;  //대화상자 응답값 반환값을 저장하기위한 변수선언

//대화상자 생성(제목텍스트, 부모위젯, 대화상자속성, 버튼이름, 응답코드, 버튼, 응답코드 ... 마지막에 NULL)
//버튼이 포함된 대화상자 위젯 생성
    dialog = gtk_dialog_new_with_buttons("고객 등록", GTK_WINDOW(frame), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_STOCK_YES, GTK_RESPONSE_YES, GTK_STOCK_NO, GTK_RESPONSE_NO, NULL);

//라벨생성-------------------------------------------------------------------    
    name_label = gtk_label_new("고객명  :");
    number_label = gtk_label_new("고객번호 :");
    phone_label = gtk_label_new("연락처  :");
    address_label = gtk_label_new("주  소 :");
    detail_label = gtk_label_new("참고사항 :");

//엔트리 위젯(한줄 입력 박스)생성-----------------------------------------------------------
    name_entry = gtk_entry_new();  //한줄의 텍스트를 입력할 수 있는 Widget 
				//GtkEntry를 생성하는 함수이다. 반환 타입은 GtkWidget의 포인터이다. 
    number_entry = gtk_entry_new();
    phone_entry = gtk_entry_new();
    address_entry = gtk_entry_new();
    detail_entry = gtk_entry_new();

//박스생성----------------------------------------------------------
    name_hbox = gtk_hbox_new(TRUE, 5);  //수평패킹상자 위젯 생성
    number_hbox = gtk_hbox_new(TRUE, 5);
    phone_hbox = gtk_hbox_new(TRUE, 5);
    address_hbox = gtk_hbox_new(TRUE, 5);
    detail_hbox = gtk_hbox_new(TRUE, 5);

//박스에 위젯 추가-----------------------------------------------------   
    gtk_box_pack_start(GTK_BOX(name_hbox), name_label, FALSE, FALSE, 5);  //H:왼쪽 V:위쪽 부터 위젯추가
								//(채울상자, 넣을위젯, T:다른위젯과사용가능한 공간 채움, 
								//T:위젯들이 남은공간 채움, 위젯둘러싼 공간여백폭)
    gtk_box_pack_start(GTK_BOX(name_hbox), name_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(number_hbox), number_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(number_hbox), number_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(phone_hbox), phone_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(phone_hbox), phone_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(address_hbox), address_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(address_hbox), address_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(detail_hbox), detail_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(detail_hbox), detail_entry, FALSE, FALSE, 5);

//다이얼로그에 박스 추가----------------------------------------------------
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), name_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), number_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), phone_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), address_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), detail_hbox, FALSE, FALSE, 5);

//대화상자의 모든위젯 화면에 출력
    gtk_widget_show_all(dialog);

//대화상자 버튼 클릭에 따른 실행(switch case문)---------------------------------------------------
//대화상자의 사용자 응답값을 반환
    result = gtk_dialog_run(GTK_DIALOG(dialog));  //사용자의 응답을 받을 때 까지 모든 실행이 중지
    switch (result){
        case GTK_RESPONSE_YES: //대화상자의 '저장'버튼을 선택했을 경우
            message_label = gtk_label_new("고객정보가 등록되었습니다");
        break;

        case GTK_RESPONSE_NO:  //대화상자의 '취소'버튼을 선택했을 경우
            message_label = gtk_label_new("고객등록이 취소되었습니다");
        break;
    }

    gtk_widget_destroy(dialog); //모달대화상자 제거
    gtk_widget_set_size_request(message_label, 150, 30);  //메시지 라벨 크기설정
    gtk_fixed_put (GTK_FIXED(frame), message_label, 250, 500);  //메시지라벨을 Fixed에 부착
}

//고객목록 생성 함수------------------------------------------------------------------------------------------------------
void view_customer_list(GtkWidget *widget, gpointer data/*GtkContainer *box*//*GtkWidget *widget, GtkFixed *frame*/){
    enum  //enum은 열을 구분하는데 유용
    {
        COLUMN_1,
        COLUMN_2,
        COLUMN_3,
        COLUMN_4,
        COLUMN_5,
        COLUMN_6,
        N_COLUMNS
    };
/*
   GtkTreeModel객체 : 데이터 저장 모델(GtkTreeStore, GtkListStroe, GtkTreeIter - 각 모델의 행을 나타냄)
   GtkTreeView위젯 : 데이터 실제 표시(GtkTreeViewColumn - 각 열을 담당,  GtkCellRenderer - 각셀의 렌더링 담당)
*/


//위젯선언------------------------------------
    GtkListStore *store;
    GtkWidget *view;
    GtkTreeIter iter1, iter2;  //모델의 행
    GtkCellRenderer *renderer;  //셀렌더러 객체

//리스트 모델--------------------------------------------------------------
    //리스트 저장 모델 생성(생성할 열의수, 각 열의 데이터타입 ...)
    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
       
    gtk_list_store_append (store, &iter1); //리스트 저장 모델에 행 추가 (리스트명, 모델행)
    gtk_list_store_set (store, &iter1, COLUMN_1, "김민지", COLUMN_2, "133289", COLUMN_3, "1", COLUMN_4, "010-2567-3308",
            COLUMN_5, "부산", COLUMN_6, "부산", -1); //특정행에 자료 설정 (리스트명, 행, 1열, 1열해당자료, ..., -1)
    gtk_list_store_append (store, &iter2);
    gtk_list_store_set (store, &iter2, COLUMN_1, "오태규", COLUMN_2, "113369", COLUMN_3, "3", COLUMN_4, "010-1234-5678",
            COLUMN_5, "부산", COLUMN_6, "부산", -1);
    
//트리뷰 위젯----------------------------------------------------------------------------------------------
    view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store)); //'store'모델에 대한 트리뷰 위젯 생성
    renderer = gtk_cell_renderer_text_new();  //셀에 텍스트 셀렌더러 객체 생성
    //트리뷰에 새로운 열 삽입(트리뷰, 위치, 타이틀, 셀렌더러, 텍스트, 해당 열이름, 널값)
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 0, "고객명", renderer, "text", COLUMN_1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 1, "고객번호", renderer, "text", COLUMN_2, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 2, "고객등급", renderer, "text", COLUMN_3, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 3, "연락처", renderer, "text", COLUMN_4, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 4, "등록지점", renderer, "text", COLUMN_5, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 5, "주소", renderer, "text", COLUMN_6, NULL);
   
    gtk_widget_set_size_request(view, 900, 350);  //리스트 뷰 크기 설정
    gtk_fixed_put (GTK_FIXED(frame), view, 0, 100);  //Fixed에 리스트 뷰 부착
    gtk_widget_show (view);  //리스트뷰 출력

//고객등록버튼----------------------------------------------------------------------------------------
    GtkWidget *register_bt = gtk_button_new_with_label("고객 등록");  //고객등록 버튼 생성
    gtk_widget_set_size_request(register_bt, 200, 50);  //고객등록 버튼 크기 설정
    gtk_fixed_put(GTK_FIXED(frame), register_bt, 600, 500);  //고객등록 버튼을 Fixed에 부착
    gtk_widget_show (register_bt);  //고객등록 버튼 출력

//고객등록버튼에 툴팁 부착-------------------------------------
    GtkTooltips *tooltips;  //툴팁선언
    tooltips = gtk_tooltips_new();  //툴팁생성
    gtk_tooltips_set_tip(tooltips, register_bt, "새로운 고객을 등록하려면 버튼을 누르세요", NULL); //버튼에 툴팁 설정
                                                                    //(툴팁명, 버튼명, 부착할 자료, 널값)
   
//고객등록 버튼 클릭시 콜백함수 호출------------------------------------------------------------------
    g_signal_connect(GTK_OBJECT(register_bt), "clicked", GTK_SIGNAL_FUNC(customer_register), NULL);
}


void *gtk() {

//위젯선언------------------------------------------------------------------------------
    GtkWidget *window;
   // GtkWidget *frame1, *frame2, *vbox, *label1, *label2;
    GtkWidget *title_label;
    GtkWidget *customer_bt;
    GtkWidget *goods_bt;
    GtkWidget *sales_bt;

//윈도우설정----------------------------------------------------------------------------
    gtk_init(NULL,NULL); //gtk환경설정
    window= gtk_window_new(GTK_WINDOW_TOPLEVEL);   //윈도우 위젯 생성
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);  //윈도우 디폴트 크기 설정
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);  //윈도우의 화면상 위치 설정
    gtk_window_set_title(GTK_WINDOW(window), "매장관리프로그램");  //제목표시줄의 텍스트 설정
    g_signal_connect (GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(quit), NULL);  //윈도우를 종료할때 콜백함수 연결

//GtkFixed생성-----------------------------------------------------------------------------
    frame = gtk_fixed_new(); //GtkFixed를 생성하는 함수이다. 필요한 인자는 없다.
    		//GtkBin이나 GtkTable처럼 한 영역에 (GtkTable의 경우에는 Cell)하나의 Widget만을 배치할 수 있는것이 아니고 
    		//마구 겹쳐넣는것도 된다
    gtk_container_add(GTK_CONTAINER(window), frame);  //기본 Window에 Fixed를 넣어준다.
    
//첫 구현화면 구성 위젯 생성-----------------------------------------------------------------
    title_label = gtk_label_new ("소공마트 1호점");  //타이틀 라벨생성
    gtk_fixed_put(GTK_FIXED(frame), title_label, 0, 10); //Widget을 GtkFixed에 포함시키는 함수이다.
                                                    //(GtkFixed 자기 자신, 포함시킬 widget, x는 가로 좌표, y는 세로 좌표)
    gtk_widget_set_size_request(title_label, 900, 40);  //GtkFixed와는 무관하게 GtkWidget에 해당하는 함수이다. 
					//GtkWidget의 최소 크기를 변경
    customer_bt = gtk_button_new_with_label("고객관리");  //고객관리 버튼생성
    gtk_fixed_put(GTK_FIXED(frame), customer_bt, 0, 50);  
    gtk_widget_set_size_request(customer_bt, 300, 40);  

    goods_bt = gtk_button_new_with_label("상품재고");  //상품재고 버튼생성
    gtk_fixed_put(GTK_FIXED(frame), goods_bt, 300, 50);
    gtk_widget_set_size_request(goods_bt, 300, 40);

    sales_bt = gtk_button_new_with_label("매출전표");  //매출전표 버튼생성
    gtk_fixed_put(GTK_FIXED(frame), sales_bt, 600, 50);
    gtk_widget_set_size_request(sales_bt, 300, 40);

//각 버튼클릭시 시그널처리 콜백함수 호출-----------------------------------------------------------------------
    g_signal_connect(GTK_OBJECT(customer_bt), "clicked", GTK_SIGNAL_FUNC(view_customer_list), NULL);  
                                        //고객관리 버튼 클릭시 시그널발생, 콜백함수 호출, 전달인자NULL
    
    gtk_widget_show_all(window);  //모든자식위젯을 화면에 나타낸다.
    gtk_main();  //메인루프를 돌면서 이벤트, 시그널을 탐지하고 콜백함수를 호출하기를 반복(무한루프를 돈다)

    return 0;
}











    
int main(int argc, char *argv[]) {
       struct sockaddr_in serv_addr;
       pthread_t snd_thread, rcv_thread,gtk_thread;
       void * thread_return;
       if(argc!=4) {
          printf("Usage : %s <IP> <port> <name>\n", argv[0]);
          exit(1);
       }
                            
        sprintf(name, "[%s]", argv[3]); 
        //sprintf(name,"[%s]",argv[3]); 인자에 넣어주는방법
        sock=socket(PF_INET, SOCK_STREAM, 0);                            
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
        serv_addr.sin_port=htons(atoi(argv[2]));
                                                     
        if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) error_handling("connect() error");
        pthread_create(&gtk_thread, NULL, gtk, NULL);
        pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
        pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
        pthread_join(snd_thread, &thread_return);
        pthread_join(rcv_thread, &thread_return);
        close(sock);   return 0;
}
//this send is message send 
void * send_msg(void * arg) {   // send thread main
     int sock=*((int*)arg);
     char name_msg[NAME_SIZE+BUF_SIZE];
     while(1) {
          fgets(msg, BUF_SIZE, stdin);
         if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")) {
                close(sock);   exit(0);
         }
         sprintf(name_msg,"%d %s", num, msg);
         //보낼 메세지 내용에 번호를 추가하여 보냄 앞에 num이 그각자의 고유 번호임
         //  printf("send section  ::%s",name_msg);
         write(sock, name_msg, strlen(name_msg));
     }
     return NULL;
}
    
//client message recv
void * recv_msg(void * arg) {  // read thread main
    int sock=*((int*)arg);
    char name_msg[NAME_SIZE+BUF_SIZE];
    int str_len;
    while(1) {
         str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
         if(str_len==1) {
             num = (int)name_msg[0];
         }
         else if(str_len==-1) return (void*)-1;
                                                    
         else {
            int chk = (int)name_msg[0]-48; 
            name_msg[str_len]=0;
            if(num == chk){
            fputs(name_msg, stdout);
            }
        }
    }
    return NULL;
}    
void error_handling(char *msg) {
        fputs(msg, stderr);
        fputc('\n', stderr);
        exit(1);
}

