#include <gtk/gtk.h>
#include <string.h>


GtkWidget *date;
GtkWidget *description;
GtkWidget *amount;
GtkWidget *type;
GtkWidget *date1;

struct Tranzactie{
       char datet[20];
       char descriptiont[100];
       int amountt;
       char typet[10];
} ;

enum {
  COLUMN_ID = 0,
  COLUMN_USER_ID,
  COLUMN_DATE,
  COLUMN_DESCRIPTION,
  COLUMN_AMMOUNT,
  COLUMN_TYPE,
  NUM_COLUMNS
};


void _init_windows(GtkWidget *window){
    //create container
    gtk_window_set_title(GTK_WINDOW(window), "Tranzactie");
    gtk_container_set_border_width(GTK_CONTAINER(window), 50);
    gtk_widget_set_size_request(window, 1000, 700);
}						

void _init_grid(GtkWidget *grid) {
    //create grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 50);
    gtk_grid_set_column_spacing(GTK_GRID(grid),50);
}


int validation_number(char x) {
	
    return x >= '0' && x <= '9';
}

int validation_char(char x) {

    return x >= 'A' && x <= 'z';
}

int date_validation() {
    const char *date_txt = gtk_entry_get_text(GTK_ENTRY(date));
    for (int i = 0; i <= 1; i ++){
        if (!validation_number(date_txt[i]))
           return 0;
    }
    if (date_txt[2] != '/')
    	return 0;
    for (int i = 3; i <= 4; i++){
    	if (!validation_number(date_txt[i]))
           return 0;
    }
    if (date_txt[5] != '/')
    	return 0;
    for (int i = 6; i <= 9; i++){
    	if (!validation_number(date_txt[i]))
           return 0;
    }
           
    return 1;
    
}

int date1_validation() {
    const char *date_txt = gtk_entry_get_text(GTK_ENTRY(date1));
    for (int i = 0; i <= 1; i ++){
        if (!validation_number(date_txt[i]))
           return 0;
    }
    if (date_txt[2] != '/')
    	return 0;
    for (int i = 3; i <= 4; i++){
    	if (!validation_number(date_txt[i]))
           return 0;
    }
    if (date_txt[5] != '/')
    	return 0;
    for (int i = 6; i <= 9; i++){
    	if (!validation_number(date_txt[i]))
           return 0;
    }
           
    return 1;
    
}


int description_validation() {
    const char *date_txt = gtk_entry_get_text(GTK_ENTRY(description));
    if (!strlen(date_txt))
    	return 0;
    for (int i = 0; i < strlen(date_txt); i++){
    	if(!validation_char(date_txt[i])){
    		return 0;
    	}
    }
    	
    return 1;
    	  
}

int amount_validation() {
    int ok = 0;
    const char *date_txt = gtk_entry_get_text(GTK_ENTRY(amount));
    for (int i = 0; i < strlen(date_txt); i++){
        if (!validation_number(date_txt[i]))
           return 0;
    }
           
    return 1;
}

int type_validation() {
    char cuv1[] = "income";
    char cuv2[] = "expense";
    int ok1 = 0;
    int ok2 = 0;
    const char *date_txt = gtk_entry_get_text(GTK_ENTRY(type));
    if (!strlen(date_txt))
    	return 0;
    if (strcmp(cuv1, date_txt) == 0){
    	ok1 = 1;
    }
    if (strcmp(cuv2, date_txt) == 0){
    	ok1 = 1;
    }
    if (ok1 == 1 || ok2 == 1){
    	return 1;
    }
    else{
    	return 0;
    }
    
}

struct Tranzactie create_transaction(char buffer[100]){
    struct Tranzactie tranzactie;
    int i = 0, j = 0;
    while(buffer[i] != '|'){
    	tranzactie.datet[j]  = buffer[i];
    	i++;
    	j++;
    }
    tranzactie.datet[j] = '\0';
    i++;
    j = 0;
    while(buffer[i] != '|'){
    	tranzactie.descriptiont[j] = buffer[i];
    	i++;
    	j++;
    }
    tranzactie.typet[j] = '\0';
    j = 0;
    i++;
    int c = 0;
    while(buffer[i] != '|'){
    	c =  c*10 + buffer[i]-'0';
    	i++;
    }
    i++;
    tranzactie.amountt = c;
    while(buffer[i] != '|'){
    	tranzactie.typet[j] = buffer[i];
    	i++;
    	j++;
    }
    tranzactie.typet[j] = '\0';
    return tranzactie;
}

int get_id_file(){
   FILE *file;
    char buffer[10000];  
    
    file = fopen("tranzacti.in", "r");
    
    int lineCount = 0;
    
    // Read the file line by line using fgets
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineCount++;
    }
    
    fclose(file);
    return lineCount;
}


int get_sum_all_entry(){

    FILE *file;
    file = fopen("tranzacti.in", "r");
    int suma = 0, j;
    char cuv1[100];
    int d = get_id_file();
    for (int i = 0; i < d; i++){
    	char buffer[100];
    	fgets(buffer, 100, file);
    	for (j = 0; buffer[j]!='\n'; j++);
    	buffer[j] = '\0';
    	struct Tranzactie tranzactie = create_transaction(buffer);
    	int c = tranzactie.amountt;
    	strcpy(cuv1, tranzactie.typet);
    	if (strcmp(cuv1, "income") == 0){
    		suma = suma + c;
    	}

    	else{
    		suma = suma - c;
    	}
    }
    return suma;
    	
    
}

void add_transaction_in_file(){
     const char *date_txt = gtk_entry_get_text(GTK_ENTRY(date));
     const char *desc_txt = gtk_entry_get_text(GTK_ENTRY(description));
     const char *amount_txt = gtk_entry_get_text(GTK_ENTRY(amount));
     const char *type_txt = gtk_entry_get_text(GTK_ENTRY(type));
     FILE *file;
     file = fopen("tranzacti.in", "a");
     //fprintf(file, "%d|", get_id_file() + 1);
     fprintf(file, "%s|%s|%s|%s|\n", date_txt, desc_txt, amount_txt, type_txt);
     fclose(file);
     
}

int data_validation(){
    int ok = 1;
    if (date_validation() == 0){
    	ok = 0;
    }
    //printf("%d", ok);
    if (description_validation() == 0){
    	ok = 0;
    }
    //printf("%d", ok);
    if (type_validation()  == 0){
     	ok = 0;
    }
    if (amount_validation()  == 0){
     	ok = 0;
    }
    return ok;
}

void save_data(GtkWidget *widget, gpointer *window){
    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "valid_data");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    
    gtk_widget_show_all(dialog);
    add_transaction_in_file();

}

void invalid_data(GtkWidget *widget, gpointer *window){

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Invalid_data");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    
    gtk_widget_show_all(dialog);
}

void click_save(GtkWidget *widget, gpointer *window){
	if (data_validation() == 1){
		save_data(widget, window);
	}
	else{
		invalid_data(widget, window);
	}
}

void save_enter( GtkWidget *grid, gpointer *window) {

     date = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(date), "Enter date here (dd/mm/yyyy)"); // Set placeholder text
     gtk_grid_attach(GTK_GRID(grid), date, 0, 0, 1, 1);
     
     description = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(description), "Enter description here"); // Set placeholder text
     gtk_grid_attach(GTK_GRID(grid), description, 0, 1, 1, 1);
     
     amount = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(amount), "Enter ammount here"); // Set placeholder text
     gtk_grid_attach(GTK_GRID(grid), amount, 0, 2, 1, 1);
     
     type = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(type), "Enter type here"); // Set placeholder text
     gtk_grid_attach(GTK_GRID(grid), type, 0, 3, 1, 1);
    
     
     GtkWidget *button3 = gtk_button_new_with_label("Salvare_tranzactie");
     int width = 150;  // Set the desired width
     int height = 150;  // Set the desired height
     gtk_widget_set_size_request(button3, width, height);
     
     gtk_grid_attach(GTK_GRID(grid), button3, 0, 4, 1, 1);

     g_signal_connect(button3, "clicked", G_CALLBACK(click_save), window);
   
}



void button_1_clicked(GtkWidget *widget, gpointer *window) {

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Adauga tranzactii");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    save_enter(grid, window);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_show_all(dialog);
    
}

void load_transaction_table(GtkWidget *container){

  FILE *file;
  file = fopen("tranzacti.in", "r");
  GtkListStore *liststore;
  liststore = gtk_list_store_new(NUM_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING);
  	
  GtkTreeIter iter;
  for(int i = 0; i<get_id_file(); i++){
        char buffer[100];
    	fgets(buffer, 100, file);
    	int j = 0;
    	for (j = 0; buffer[j]!='\n'; j++);
    	buffer[j] = '\0';
    	struct Tranzactie tranzactie = create_transaction(buffer);
    
    // create the list of columns;
        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter,
    	COLUMN_ID, i,
 	COLUMN_DATE, tranzactie.datet, 
    	COLUMN_DESCRIPTION, tranzactie.descriptiont,
    	COLUMN_AMMOUNT, tranzactie.amountt,
    	COLUMN_TYPE, tranzactie.typet);
  }
  
  GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
  gtk_grid_attach(GTK_GRID(container), treeview, 1, 1, 1, 1);
  
  
  for(int i = 0; i < NUM_COLUMNS; i++){
  
    // dau render la fiecare coloana
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(cols[i],
    	renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column); 
  }
  
  GtkWidget *l1;
  l1 = gtk_label_new("");
  gtk_grid_attach(GTK_GRID(container), l1, 1, 2, 1, 1);
  
}

void button_2_clicked(GtkWidget *widget, gpointer *window) {

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Dialog");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);
    load_transaction_table(grid);
    
    gtk_widget_show_all(dialog);
    
}

void button_4_clicked(GtkWidget *widget, gpointer *window) {

   GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Balanta contului");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    int my_integer = get_sum_all_entry();

    // Convert the integer to a string for display
    gchar *int_str = g_strdup_printf("%d", my_integer);

    // Create a label to display the integer
    GtkWidget *label = gtk_label_new(int_str);
    g_free(int_str);

    // Attach the label to the grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    
    gtk_widget_show_all(dialog);
   
}

int compara(char cuv2[]){
     const char *date_txt = gtk_entry_get_text(GTK_ENTRY(date));
     const char *date1_txt = gtk_entry_get_text(GTK_ENTRY(date1));
     int a = 0, b = 0, c = 0;
     for(int i = 6; i <= 9; i++){
     	a = a *10 + date_txt[i]-'0';
     	b = b*10 + cuv2[i]-'0';
     	c = c*10 + date1_txt[i] - '0';
     }
     //printf("%d|%d|%d\n", a, b, c);
     if (a > b || b > c)
     	return 0;
     if(a == b){
     	for(int i = 3; i <= 4; i++){
     	    a = a *10 + date_txt[i]-'0';
     	    b = b*10 + cuv2[i]-'0';
     	}
     	if(a > b){
     	   return 0;
     	}
     	for(int i = 0; i <= 1; i++){
     	    a = a *10 + date_txt[i]-'0';
     	    b = b*10 + cuv2[i]-'0';
     	}
     	if(a > b){
     	   return 0;
     	}
     }
     if(b == c){
     	for(int i = 3; i <= 4; i++){
     	    c = c *10 + date1_txt[i]-'0';
     	    b = b*10 + cuv2[i]-'0';
     	}
     	if(b > c){
     	   return 0;
     	}
     	for(int i = 0; i <= 1; i++){
     	    c = c *10 + date1_txt[i]-'0';
     	    b = b*10 + cuv2[i]-'0';
     	}
     	if(b > c){
     	   return 0;
     	}
     }
     return 1;
     	
     
     
}

int gaseste(){
    FILE *file;
    file = fopen("tranzacti.in", "r");
    int suma = 0, j;
    char cuv1[100];
    char cuv2[100];
    int d = get_id_file();
    for (int i = 0; i < d; i++){
    	char buffer[100];
    	fgets(buffer, 100, file);
    	for (j = 0; buffer[j]!='\n'; j++);
    	buffer[j] = '\0';
    	struct Tranzactie tranzactie = create_transaction(buffer);
    	int c = tranzactie.amountt;
    	strcpy(cuv1, tranzactie.typet);
    	strcpy(cuv2, tranzactie.datet);
    	printf("%s\n", cuv2);
    	if (strcmp(cuv1, "income") == 0){
    	    if(compara(cuv2) == 1){
    	    	suma = suma + c;
    	    	printf("%s\n", "aeiou");
    	    }
    	}

    }
    return suma;
}


void printeaza_venituri(GtkWidget *widget, gpointer *window){

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Venituri Peridoada");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
     GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    int my_integer = gaseste();

    // Convert the integer to a string for display
    gchar *int_str = g_strdup_printf("%d", my_integer);

    // Create a label to display the integer
    GtkWidget *label = gtk_label_new(int_str);
    g_free(int_str);

    // Attach the label to the grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    
    
    gtk_widget_show_all(dialog);
}

void venituri_perioada(GtkWidget *widget, gpointer *window){
     int ok = 1;
     if (date1_validation() == 1){
        if (date_validation() == 1){
           printeaza_venituri(widget, window);
        }
        else 
            ok = 0;
     }
     else
     	ok = 0;
     	
     if (ok == 0){
         invalid_data(widget, window);
     }
}

void venituri(GtkWidget *widget, gpointer *window){

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Venituri");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);
    
     date = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(date), "Enter date here (dd/mm/yyyy)"); // Set placeholder text
     gtk_grid_attach(GTK_GRID(grid), date, 0, 0, 1, 1);
     
     date1 = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(date1), "Enter date here (dd/mm/yyyy)"); // Set placeholder text
     gtk_grid_attach(GTK_GRID(grid), date1, 0, 1, 1, 1);
     
     GtkWidget *button8 = gtk_button_new_with_label("Venituri in perioada intordusa");
     int width = 150;  // Set the desired width
     int height = 150;  // Set the desired height
     gtk_widget_set_size_request(button8, width, height);
     
     gtk_grid_attach(GTK_GRID(grid), button8, 0, 2, 1, 1);

     g_signal_connect(button8, "clicked", G_CALLBACK(venituri_perioada), window);
   
    
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_show_all(dialog);
     

}

int gaseste1(){
    FILE *file;
    file = fopen("tranzacti.in", "r");
    int suma = 0, j;
    char cuv1[100];
    char cuv2[100];
    int d = get_id_file();
    for (int i = 0; i < d; i++){
    	char buffer[100];
    	fgets(buffer, 100, file);
    	for (j = 0; buffer[j]!='\n'; j++);
    	buffer[j] = '\0';
    	struct Tranzactie tranzactie = create_transaction(buffer);
    	int c = tranzactie.amountt;
    	strcpy(cuv1, tranzactie.typet);
    	strcpy(cuv2, tranzactie.datet);
    	printf("%s\n", cuv2);
    	if (strcmp(cuv1, "expense") == 0){
    	    if(compara(cuv2) == 1){
    	    	suma = suma + c;
    	    	printf("%s\n", "aeiou");
    	    }
    	}

    }
    return suma;
}

void printeaza_cheltuieli(GtkWidget *widget, gpointer *window){

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Cheltuieli Peridoada");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
     GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    int my_integer = gaseste1();

    // Convert the integer to a string for display
    gchar *int_str = g_strdup_printf("%d", my_integer);

    // Create a label to display the integer
    GtkWidget *label = gtk_label_new(int_str);
    g_free(int_str);

    // Attach the label to the grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    
    
    gtk_widget_show_all(dialog);
}

void cheltuieli_perioada(GtkWidget *widget, gpointer *window){
     int ok = 1;
     if (date1_validation() == 1){
        if (date_validation() == 1){
           printeaza_cheltuieli(widget, window);
        }
        else 
            ok = 0;
     }
     else
     	ok = 0;
     	
     if (ok == 0){
         invalid_data(widget, window);
     }
}

void cheltuieli(GtkWidget *widget, gpointer *window){

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "cheltuieli");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);
    
     date = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(date), "Enter date here (dd/mm/yyyy)"); // Set placeholder text
     gtk_grid_attach(GTK_GRID(grid), date, 0, 0, 1, 1);
     
     date1 = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(date1), "Enter date here (dd/mm/yyyy)"); // Set placeholder text
     gtk_grid_attach(GTK_GRID(grid), date1, 0, 1, 1, 1);
     
     GtkWidget *button8 = gtk_button_new_with_label("Cheltuieli in perioada intordusa");
     int width = 150;  // Set the desired width
     int height = 150;  // Set the desired height
     gtk_widget_set_size_request(button8, width, height);
     
     gtk_grid_attach(GTK_GRID(grid), button8, 0, 2, 1, 1);

     g_signal_connect(button8, "clicked", G_CALLBACK(cheltuieli_perioada), window);
   
    
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_show_all(dialog);
     

}

void button_5_clicked(GtkWidget *widget, gpointer *window) {

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Raporturi financiare");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    
     GtkWidget *button6 = gtk_button_new_with_label("Rapoarte venituri");
     GtkWidget *button7 = gtk_button_new_with_label("Rapoarte cheltuieli");
     int width = 150;  // Set the desired width
     int height = 150;  // Set the desired height
     gtk_widget_set_size_request(button6, width, height);
     gtk_widget_set_size_request(button7, width, height);
     
     gtk_grid_attach(GTK_GRID(grid), button6, 0, 0, 1, 1);
     gtk_grid_attach(GTK_GRID(grid), button7, 0, 20, 1, 1);

     g_signal_connect(button6, "clicked", G_CALLBACK(venituri), window);
     g_signal_connect(button7, "clicked", G_CALLBACK(cheltuieli), window);
    
    gtk_widget_show_all(dialog);
   
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    _init_windows(window);

    // Create a grid to hold the data
    GtkWidget *grid = gtk_grid_new();
    _init_grid(grid);


    // Create the first button
    GtkWidget *button1 = gtk_button_new_with_label("Adaugare_tranzactie");
    g_signal_connect(button1, "clicked", G_CALLBACK(button_1_clicked), window);
    
     
    //Create the second button
    GtkWidget *button2 = gtk_button_new_with_label("Afisare_tranzactie");
    g_signal_connect(button2, "clicked", G_CALLBACK(button_2_clicked), window);
    
    //Create a button
    GtkWidget *button4 = gtk_button_new_with_label("Balanta contului");
    g_signal_connect(button4, "clicked", G_CALLBACK(button_4_clicked), window);
    
    //Create a button
    GtkWidget *button5 = gtk_button_new_with_label("Raporturi financiare");
    g_signal_connect(button5, "clicked", G_CALLBACK(button_5_clicked), window);
    
    
    // Set the button size
    int width = 150;  // Set the desired width
    int height = 150;  // Set the desired height
    gtk_widget_set_size_request(button1, width, height);
    gtk_widget_set_size_request(button2, width, height);
    gtk_widget_set_size_request(button4, width, height);
    gtk_widget_set_size_request(button5, width, height);
    
    

    // Create a vertical box to hold the grid and the save button
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
    gtk_container_add(GTK_CONTAINER(vbox), grid);
    gtk_container_add(GTK_CONTAINER(vbox), button1);
    gtk_container_add(GTK_CONTAINER(vbox), button2);
    gtk_container_add(GTK_CONTAINER(vbox), button4);
    gtk_container_add(GTK_CONTAINER(vbox), button5);

    // Add the vbox to the main window
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Connect the "destroy" event to quit the GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
