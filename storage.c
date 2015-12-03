#include <gtk/gtk.h>

GtkWidget *frame; //GtkFixed를 Global로 선언한다.
		//Callback 함수에서 여러개의 Widget을 사용할 일이 있어서이다.


//상품등록 대화상자 생성 함수----------------------------------------------------------------------------------------------
void product_register(GtkWidget *widget, gpointer data){
//위젯 선언----------------------------------------------------------------------
    GtkWidget *dialog;
    GtkWidget *name_label, *number_label, *selling_price_label, *unit_price_label, *amount_label;
    GtkWidget *name_entry, *number_entry, *selling_price_entry, *unit_price_entry, *amount_entry;
    GtkWidget *name_hbox, *number_hbox, *selling_price_hbox, *unit_price_hbox, *amount_hbox;
    GtkWidget *message_label;
    
    gint result;  //대화상자 응답값 반환값을 저장하기위한 변수선언

//대화상자 생성(제목텍스트, 부모위젯, 대화상자속성, 버튼이름, 응답코드, 버튼, 응답코드 ... 마지막에 NULL)
//버튼이 포함된 대화상자 위젯 생성
    dialog = gtk_dialog_new_with_buttons("상품 등록", NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
            "등록", GTK_RESPONSE_YES, "취소", GTK_RESPONSE_NO, NULL);

//라벨생성-------------------------------------------------------------------    
    name_label = gtk_label_new("상품명  :");
    number_label = gtk_label_new("상품번호 :");
    selling_price_label = gtk_label_new("판매가  :");
    unit_price_label = gtk_label_new("단  가 :");
    amount_label = gtk_label_new("입수수량 :");

//엔트리 위젯(한줄 입력 박스)생성-----------------------------------------------------------
    name_entry = gtk_entry_new();  //한줄의 텍스트를 입력할 수 있는 Widget 
				//GtkEntry를 생성하는 함수이다. 반환 타입은 GtkWidget의 포인터이다. 
    number_entry = gtk_entry_new();
    selling_price_entry = gtk_entry_new();
    unit_price_entry = gtk_entry_new();
    amount_entry = gtk_entry_new();

//박스생성----------------------------------------------------------
    name_hbox = gtk_hbox_new(TRUE, 5);  //수평패킹상자 위젯 생성
    number_hbox = gtk_hbox_new(TRUE, 5);
    selling_price_hbox = gtk_hbox_new(TRUE, 5);
    unit_price_hbox = gtk_hbox_new(TRUE, 5);
    amount_hbox = gtk_hbox_new(TRUE, 5);

//박스에 위젯 추가-----------------------------------------------------   
    gtk_box_pack_start(GTK_BOX(name_hbox), name_label, FALSE, FALSE, 5);  //H:왼쪽 V:위쪽 부터 위젯추가
								//(채울상자, 넣을위젯, T:다른위젯과사용가능한 공간 채움, 
								//T:위젯들이 남은공간 채움, 위젯둘러싼 공간여백폭)
    gtk_box_pack_start(GTK_BOX(name_hbox), name_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(number_hbox), number_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(number_hbox), number_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(selling_price_hbox), selling_price_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(selling_price_hbox), selling_price_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(unit_price_hbox), unit_price_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(unit_price_hbox), unit_price_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(amount_hbox), amount_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(amount_hbox), amount_entry, FALSE, FALSE, 5);

//다이얼로그에 박스 추가----------------------------------------------------
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), name_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), number_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), selling_price_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), unit_price_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), amount_hbox, FALSE, FALSE, 5);

//대화상자의 모든위젯 화면에 출력
    gtk_widget_show_all(dialog);

//대화상자 버튼 클릭에 따른 실행(switch case문)---------------------------------------------------
//대화상자의 사용자 응답값을 반환
    result = gtk_dialog_run(GTK_DIALOG(dialog));  //사용자의 응답을 받을 때 까지 모든 실행이 중지
    switch (result){
        case GTK_RESPONSE_YES: //대화상자의 '주문'버튼을 선택했을 경우
            message_label = gtk_label_new("새 상품이 등록되었습니다");
        break;

        case GTK_RESPONSE_NO:  //대화상자의 '취소'버튼을 선택했을 경우
            message_label = gtk_label_new("상품등록이 취소되었습니다");
        break;
    }

    gtk_widget_destroy(dialog); //모달대화상자 제거
    gtk_widget_set_size_request(message_label, 150, 30);  //메시지 라벨 크기설정
    gtk_fixed_put (GTK_FIXED(frame), message_label, 250, 500);  //메시지라벨을 Fixed에 부착
}

//상품목록 생성 함수------------------------------------------------------------------------------------------------------
void view_product_list(GtkWidget *widget, gpointer data){
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

    //위젯선언------------------------------------
    GtkListStore *store;
    GtkWidget *view;
    GtkTreeIter iter1, iter2;  //모델의 행
    GtkCellRenderer *renderer;  //셀렌더러 객체

    //리스트 모델--------------------------------------------------------------
    //리스트 저장 모델 생성(생성할 열의수, 각 열의 데이터타입 ...)
    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    gtk_list_store_append (store, &iter1); //리스트 저장 모델에 행 추가 (리스트명, 모델행)
    gtk_list_store_set (store, &iter1, COLUMN_1, "세탁세제", COLUMN_2, "123", COLUMN_3, "10,000원", COLUMN_4, "15,000원", COLUMN_5, "1000", 
	COLUMN_6, NULL, -1); 
                                            //특정행에 자료 설정 (리스트명, 행, 1열, 1열해당자료, ..., -1)
    gtk_list_store_append (store, &iter2);
    gtk_list_store_set (store, &iter2, COLUMN_1, "섬유린스", COLUMN_2, "456", COLUMN_3, "12,000원", COLUMN_4, "17,000원", COLUMN_5, "1200",
	COLUMN_6, NULL, -1);

    //트리뷰 위젯------------------------------------------------------------------------
    view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));  //'store'모델에 대한 트리뷰 위젯 생성
    renderer = gtk_cell_renderer_text_new();  //셀에 텍스트 셀렌더러 객체 생성
    
    //트리뷰에 새로운 열 삽입(트리뷰, 위치, 타이틀, 셀렌더러, 텍스트, 해당 열이름, 널값)
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 0, "상품명", renderer, "text", COLUMN_1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 1, "상품번호", renderer, "text", COLUMN_2, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 2, "판매가", renderer, "text", COLUMN_3, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 3, "단가", renderer, "text", COLUMN_4, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 4, "재고", renderer, "text", COLUMN_5, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 5, "비고", renderer, "text", COLUMN_6, NULL);

    gtk_widget_set_size_request(view, 900, 350);  //리스트 뷰 크기 설정
    gtk_fixed_put (GTK_FIXED(frame), view, 0, 100);  //Fixed에 리스트 뷰 부착
    gtk_widget_show (view);  //리스트뷰 출력

    //상품등록 버튼-----------------------------------------------------------------------------
    GtkWidget *product_register_bt = gtk_button_new_with_label("상품 등록");  //상품등록 버튼 생성
    gtk_widget_set_size_request(product_register_bt, 200, 50);  //상품등록 버튼 크기 설정
    gtk_fixed_put(GTK_FIXED(frame), product_register_bt, 600, 500);  //상품등록 버튼을 Fixed에 부착
    gtk_widget_show (product_register_bt);  //상품등록 버튼 출력

    //상품등록버튼에 툴팁 부착-------------------------------------
    GtkTooltips *tooltips;  //툴팁선언
    tooltips = gtk_tooltips_new();  //툴팁생성
    gtk_tooltips_set_tip(tooltips, product_register_bt, "새 상품 등록을 원할 시 버튼을 누르세요", NULL); //버튼에 툴팁 설정
                                                                                 //(툴팁명, 버튼명, 부착할 자료, 널값)

    //상품주문 버튼 클릭시 콜백함수 호출------------------------------------------------------------------
    g_signal_connect(GTK_OBJECT(product_register_bt), "clicked", GTK_SIGNAL_FUNC(product_register), NULL);
}


//납품내역 생성 함수----------------------------------------------------------------------------------------------------------
void view_delivery_list(GtkWidget *widget, gpointer data){
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

    //위젯선언------------------------------------
    GtkListStore *store;
    GtkWidget *view;
    GtkTreeIter iter1, iter2, iter3;  //모델의 행
    GtkCellRenderer *renderer;  //셀렌더러 객체
    
    //리스트 모델--------------------------------------------------------------
    //리스트 저장 모델 생성(생성할 열의수, 각 열의 데이터타입 ...)
    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
            G_TYPE_STRING, G_TYPE_STRING);

    gtk_list_store_append (store, &iter1); //리스트 저장 모델에 행 추가 (리스트명, 모델행) 
    gtk_list_store_set (store, &iter1, COLUMN_1, "1호점", COLUMN_2, "2015-12-01", COLUMN_3, "섬유린스", COLUMN_4,
            "30", COLUMN_5, "360,000원", COLUMN_6, NULL, -1); 
                                                        //특정행에 자료 설정 (리스트명, 행, 1열, 1열해당자료, ..., -1)
    gtk_list_store_append (store, &iter2);
    gtk_list_store_set (store, &iter2, COLUMN_1, "2호점", COLUMN_2, "2015-12-02", COLUMN_3, "세탁세제", COLUMN_4,
            "20", COLUMN_5, "200,000원", COLUMN_6, NULL, -1);

    gtk_list_store_append (store, &iter3);
    gtk_list_store_set (store, &iter3, COLUMN_1, "1호점", COLUMN_2, "2015-12-02", COLUMN_3, "세탁세제", COLUMN_4,
            "30", COLUMN_5, "300,000원", COLUMN_6, NULL, -1);

    //트리뷰 위젯------------------------------------------------------------------------

    view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));  //'store'모델에 대한 트리뷰 위젯 생성
    renderer = gtk_cell_renderer_text_new();  //셀에 텍스트 셀렌더러 객체 생성

    //트리뷰에 새로운 열 삽입(트리뷰, 위치, 타이틀, 셀렌더러, 텍스트, 해당 열이름, 널값)
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 0, "거래처", renderer, "text", COLUMN_1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 1, "납품일시", renderer, "text", COLUMN_2, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 2, "상품명", renderer, "text", COLUMN_3, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 3, "납품수량", renderer, "text", COLUMN_4, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 4, "주문금액", renderer, "text", COLUMN_5, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 5, "비고", renderer, "text", COLUMN_6, NULL);

    gtk_widget_set_size_request(view, 900, 500);  //리스트 뷰 크기 설정
    gtk_fixed_put (GTK_FIXED(frame), view, 0, 100);  //Fixed에 리스트 뷰 부착
    gtk_widget_show (view);  //리스트뷰 출력

}

//시그널종료 콜백함수---------------------------------------------------------------------------------------------
void quit(GtkWidget *window, gpointer data){
    gtk_main_quit();
}

int main (int argc, char *argv[]) {

    //위젯선언------------------------------------------------------------------------------
    GtkWidget *window;
    GtkWidget *title;
    GtkWidget *market_bt;
    GtkWidget *product_bt;
    GtkWidget *delivery_bt;

//윈도우설정----------------------------------------------------------------------------
    gtk_init(&argc, &argv); //gtk환경설정
    window= gtk_window_new(GTK_WINDOW_TOPLEVEL);   //윈도우 위젯 생성
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);  //윈도우 디폴트 크기 설정
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);  //윈도우의 화면상 위치 설정
    gtk_window_set_title(GTK_WINDOW(window), "거래처관리프로그램");  //제목표시줄의 텍스트 설정
    g_signal_connect (GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(quit), NULL);  //윈도우를 종료할때 콜백함수 연결

//GtkFixed생성-----------------------------------------------------------------------------
    frame = gtk_fixed_new(); //GtkFixed를 생성하는 함수이다. 필요한 인자는 없다.
    		//GtkBin이나 GtkTable처럼 한 영역에 (GtkTable의 경우에는 Cell)하나의 Widget만을 배치할 수 있는것이 아니고 
    		//마구 겹쳐넣는것도 된다
    gtk_container_add(GTK_CONTAINER(window), frame);  //기본 Window에 Fixed를 넣어준다.
    
//첫 구현화면 구성 위젯 생성-----------------------------------------------------------------
    title = gtk_label_new ("소공마트 거래처 관리");  //타이틀 라벨생성
    gtk_fixed_put(GTK_FIXED(frame), title, 0, 10); //Widget을 GtkFixed에 포함시키는 함수이다.
                                                    //(GtkFixed 자기 자신, 포함시킬 widget, x는 가로 좌표, y는 세로 좌표)
    gtk_widget_set_size_request(title, 900, 40);  //GtkFixed와는 무관하게 GtkWidget에 해당하는 함수이다. 
					//GtkWidget의 최소 크기를 변경
    market_bt = gtk_button_new_with_label("거래처관리");  //고객관리 버튼생성
    gtk_fixed_put(GTK_FIXED(frame), market_bt, 0, 50);  
    gtk_widget_set_size_request(market_bt, 300, 40);  

    product_bt = gtk_button_new_with_label("상품관리");  //상품재고 버튼생성
    gtk_fixed_put(GTK_FIXED(frame), product_bt, 300, 50);
    gtk_widget_set_size_request(product_bt, 300, 40);

    delivery_bt = gtk_button_new_with_label("납품내역");  //매출전표 버튼생성
    gtk_fixed_put(GTK_FIXED(frame), delivery_bt, 600, 50);
    gtk_widget_set_size_request(delivery_bt, 300, 40);

//각 버튼클릭시 시그널처리 콜백함수 호출-----------------------------------------------------------------------
    //g_signal_connect(GTK_OBJECT(market_bt), "clicked", GTK_SIGNAL_FUNC(view_customer_list), NULL);  
                                        //거래처관리 버튼 클릭시 시그널발생, 콜백함수 호출, 전달인자NULL
    g_signal_connect(GTK_OBJECT(product_bt), "clicked", GTK_SIGNAL_FUNC(view_product_list), NULL);
                                        //상품관리 버튼 클릭시 시그널발생, 콜백함수 호출, 전달인자NULL
    g_signal_connect(GTK_OBJECT(delivery_bt), "clicked", GTK_SIGNAL_FUNC(view_delivery_list), NULL);
                                        //납품내역 버튼 클릭시 시그널발생, 콜백함수 호출, 전달인자NULL

    gtk_widget_show_all(window);  //모든자식위젯을 화면에 나타낸다.
    gtk_main();  //메인루프를 돌면서 이벤트, 시그널을 탐지하고 콜백함수를 호출하기를 반복(무한루프를 돈다)

    return 0;
}
