#include <gtk/gtk.h>

GtkWidget *frame;

void quit(GtkWidget *window, gpointer data){
    gtk_main_quit();
}

void customer_register(GtkWidget *widget, gpointer data){
    GtkWidget *dialog;
    GtkWidget *name_label, *number_label, *phone_label, *address_label, *detail_label;
    GtkWidget *name_entry, *number_entry, *phone_entry, *address_entry, *detail_entry;
    GtkWidget *name_hbox, *number_hbox, *phone_hbox, *address_hbox, *detail_hbox;
    GtkWidget *message_label;
    gint result;

    dialog = gtk_dialog_new_with_buttons("고객 등록", GTK_WINDOW(frame), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_STOCK_YES, GTK_RESPONSE_YES, GTK_STOCK_NO, GTK_RESPONSE_NO, NULL);
    
    name_label = gtk_label_new("고객명  :");
    number_label = gtk_label_new("고객번호 :");
    phone_label = gtk_label_new("연락처  :");
    address_label = gtk_label_new("주  소 :");
    detail_label = gtk_label_new("참고사항 :");

    name_entry = gtk_entry_new();
    number_entry = gtk_entry_new();
    phone_entry = gtk_entry_new();
    address_entry = gtk_entry_new();
    detail_entry = gtk_entry_new();

    name_hbox = gtk_hbox_new(TRUE, 5);
    number_hbox = gtk_hbox_new(TRUE, 5);
    phone_hbox = gtk_hbox_new(TRUE, 5);
    address_hbox = gtk_hbox_new(TRUE, 5);
    detail_hbox = gtk_hbox_new(TRUE, 5);
   
    gtk_box_pack_start(GTK_BOX(name_hbox), name_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(name_hbox), name_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(number_hbox), number_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(number_hbox), number_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(phone_hbox), phone_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(phone_hbox), phone_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(address_hbox), address_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(address_hbox), address_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(detail_hbox), detail_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(detail_hbox), detail_entry, FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), name_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), number_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), phone_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), address_hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), detail_hbox, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    result = gtk_dialog_run(GTK_DIALOG(dialog));
    switch (result){
        case GTK_RESPONSE_YES:
            message_label = gtk_label_new("고객정보가 등록되었습니다");
        break;

        case GTK_RESPONSE_NO:
            message_label = gtk_label_new("고객등록이 취소되었습니다");
        break;
    }

    gtk_widget_destroy(dialog);
    gtk_widget_set_size_request(message_label, 150, 30);
    gtk_fixed_put (GTK_FIXED(frame), message_label, 250, 500);
}

void view_customer_list(GtkWidget *widget, gpointer data/*GtkContainer *box*//*GtkWidget *widget, GtkFixed *frame*/){
    enum
    {
        COLUMN_1,
        COLUMN_2,
        COLUMN_3,
        COLUMN_4,
        COLUMN_5,
        COLUMN_6,
        N_COLUMNS
    };

    GtkListStore *store;
    GtkWidget *view;
    GtkTreeIter iter1, iter2;
    GtkCellRenderer *renderer;

    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
       
    gtk_list_store_append (store, &iter1);
    gtk_list_store_set (store, &iter1, COLUMN_1, "김민지", COLUMN_2, "133289", COLUMN_3, "1", COLUMN_4, "010-2567-3308",
            COLUMN_5, "부산", COLUMN_6, "부산", -1);
    gtk_list_store_append (store, &iter2);
    gtk_list_store_set (store, &iter2, COLUMN_1, "오태규", COLUMN_2, "113369", COLUMN_3, "3", COLUMN_4, "010-1234-5678",
            COLUMN_5, "부산", COLUMN_6, "부산", -1);
    
    view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 0, "고객명", renderer, "text", COLUMN_1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 1, "고객번호", renderer, "text", COLUMN_2, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 2, "고객등급", renderer, "text", COLUMN_3, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 3, "연락처", renderer, "text", COLUMN_4, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 4, "등록지점", renderer, "text", COLUMN_5, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 5, "주소", renderer, "text", COLUMN_6, NULL);
    //gtk_container_add(GTK_CONTAINER((GtkWidget *) frame), view);
    //gtk_fixed_put(/*GTK_FIXED((GtkWidget *)*/frame, view, 10, 10);
    //gtk_widget_set_size_request(view, 850, 300);
    //gtk_container_add(box, view);
    gtk_widget_set_size_request(view, 900, 350);
    gtk_fixed_put (GTK_FIXED(frame), view, 0, 100);
    gtk_widget_show (view);
    
    GtkWidget *register_bt = gtk_button_new_with_label("고객 등록");
    gtk_widget_set_size_request(register_bt, 200, 50);
    gtk_fixed_put(GTK_FIXED(frame), register_bt, 600, 500);
    gtk_widget_show (register_bt);

    GtkTooltips *tooltips;
    tooltips = gtk_tooltips_new();
    gtk_tooltips_set_tip(tooltips, register_bt, "새로운 고객을 등록하려면 버튼을 누르세요", NULL);
   
    g_signal_connect(GTK_OBJECT(register_bt), "clicked", GTK_SIGNAL_FUNC(customer_register), NULL);
}


int main (int argc, char *argv[]) {
    GtkWidget *window;
   // GtkWidget *frame1, *frame2, *vbox, *label1, *label2;
    GtkWidget *title_label;
    GtkWidget *customer_bt;
    GtkWidget *goods_bt;
    GtkWidget *sales_bt;

    gtk_init(&argc, &argv);
    window= gtk_window_new(GTK_WINDOW_TOPLEVEL); 
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "매장관리프로그램");
    g_signal_connect (GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(quit), NULL);

    frame = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), frame);
    
    //frame2 = gtk_fixed_new();
    //gtk_fixed_put(GTK_FIXED(frame1), frame2, 0, 100);


    //vbox = gtk_vbox_new(FALSE, 5);
    //gtk_container_add(GTK_CONTAINER(frame2), vbox);
    //label1 = gtk_label_new ("ddd");
    //label2 = gtk_label_new ("sss");
    //gtk_box_pack_start(GTK_BOX(vbox), label1, TRUE, FALSE,1);
    //gtk_box_pack_start(GTK_BOX(vbox), label2, TRUE, FALSE,1);
 

    title_label = gtk_label_new ("소공마트 1호점");
    gtk_fixed_put(GTK_FIXED(frame), title_label, 0, 10); 
    gtk_widget_set_size_request(title_label, 900, 40);

    customer_bt = gtk_button_new_with_label("고객관리");
    gtk_fixed_put(GTK_FIXED(frame), customer_bt, 0, 50);
    gtk_widget_set_size_request(customer_bt, 300, 40);

    goods_bt = gtk_button_new_with_label("상품재고");
    gtk_fixed_put(GTK_FIXED(frame), goods_bt, 300, 50);
    gtk_widget_set_size_request(goods_bt, 300, 40);

    sales_bt = gtk_button_new_with_label("매출전표");
    gtk_fixed_put(GTK_FIXED(frame), sales_bt, 600, 50);
    gtk_widget_set_size_request(sales_bt, 300, 40);
 
    g_signal_connect(GTK_OBJECT(customer_bt), "clicked", GTK_SIGNAL_FUNC(view_customer_list), NULL);
    
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

