#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

typedef struct {
    GtkWidget *id_entry;
    GtkWidget *name_entry;
    GtkWidget *password_entry;
    GtkWidget *type_entry;
    GtkWidget *amount_entry;
    GtkWidget *typecont_entry;
    GtkWidget *numedest_entry;
    GtkWidget *typecontd_entry;
    
} EntryWidgets;


#define MAX_NAME_LENGTH 100

void finalizare(GtkWidget *widget, EntryWidgets *date, gpointer *window){
    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Tranzactie efectuata cu succes!");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    gtk_widget_show_all(dialog);
    
}

int validare_numar(const char *numar){
     if (isdigit(numar)){
     	return 1;
     }
     return 0;
}

void createPersonalFileTransferuri(const char *name) {
    FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%stransferuri.txt", name);

    // Check if the file exists
    if (access(filename, F_OK) == 0) {
        //printf("File '%s' already exists.\n", filename);
        return;
    }

    // Opening the file in write mode
    file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }

    // Closing the file
    fclose(file);

}

void finalizare_depozit(GtkWidget *widget, EntryWidgets *date, gpointer *window){
     EntryWidgets *data = (EntryWidgets *)date;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(data->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(data->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(data->name_entry));
    const gchar *typecont = gtk_entry_get_text(GTK_ENTRY(data->typecont_entry));
     FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%sconturi.txt", name);
    // Opening the file in read mode
    file = fopen(filename, "r");
    FILE *tempFile = fopen("temp_file.txt", "w");
    
     if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }
    
    char buffer[100];
    int found = 0;
    long pos;
    int updatedObject;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char stored_type[50],  stored_amount[50];
        if (sscanf(buffer, "%s %s", stored_type, stored_amount) == 2) {
            if (strcmp(typecont, stored_type) == 0 ) {
                found = 1;
                updatedObject = atoi(stored_amount) + atoi(amount);
                fprintf(tempFile, "%s %d\n", typecont, updatedObject);
            }
            else{
            	fprintf(tempFile, "%s %s\n", stored_type, stored_amount);
            }
        }
    }
    file = fopen(filename, "a");
    if (found) {
       remove(filename); // Delete the original file
        rename("temp_file.txt", filename);
    } else {
        printf("Deposit not found.\n");
    }
    
     fclose(file);
     finalizare(widget, data, window);
}

void transfer_depozit(GtkWidget *widget, EntryWidgets *data, gpointer *window){
      EntryWidgets *entries = (EntryWidgets *)data;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(entries->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Alege contul pentru depunerea banilor");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
   GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);

    GtkWidget *typecont_label = gtk_label_new("Cont:");
    gtk_grid_attach(GTK_GRID(grid), typecont_label, 0, 0, 1, 1);

    GtkWidget *typecont_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), typecont_entry, 1, 0, 1, 1);

    entries->typecont_entry = typecont_entry;
    
    
    GtkWidget *button = gtk_button_new_with_label("Salvare");
    int width = 150;  // Set the desired width
    int height = 150;  // Set the desired height
    
    
    gtk_widget_set_size_request(button, width, height);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(finalizare_depozit), entries);
    
    
      gtk_widget_show_all(dialog);
}

void finalizare_plata(GtkWidget *widget, EntryWidgets *date, gpointer *window){
     EntryWidgets *data = (EntryWidgets *)date;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(data->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(data->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(data->name_entry));
    const gchar *typecont = gtk_entry_get_text(GTK_ENTRY(data->typecont_entry));
     FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%sconturi.txt", name);
    // Opening the file in read mode
    file = fopen(filename, "r");
    FILE *tempFile = fopen("temp_file.txt", "w");
    
     if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }
    
    char buffer[100];
    int found = 0;
    int mic = 0;
    long pos;
    int updatedObject;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char stored_type[50],  stored_amount[50];
        if (sscanf(buffer, "%s %s", stored_type, stored_amount) == 2) {
            if (strcmp(typecont, stored_type) == 0 ) {
                found = 1;
                updatedObject = atoi(stored_amount) - atoi(amount);
                if (updatedObject < 0 ) {
                   mic = 1;
                }
                fprintf(tempFile, "%s %d\n", typecont, updatedObject);
            }
            else{
            	fprintf(tempFile, "%s %s\n", stored_type, stored_amount);
            }
        }
    }
    file = fopen(filename, "a");
    if (found && !mic) {
       remove(filename); // Delete the original file
        rename("temp_file.txt", filename);
    } else {
        printf("Deposit not found or the sum is too big.\n");
    }
    
     fclose(file);
     finalizare(widget, data, window);
}

void finalizare_transfer(GtkWidget *widget, EntryWidgets *date, gpointer *window){
     EntryWidgets *data = (EntryWidgets *)date;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(data->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(data->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(data->name_entry));
    const gchar *typecontd = gtk_entry_get_text(GTK_ENTRY(data->typecontd_entry));
    const gchar *nume = gtk_entry_get_text(GTK_ENTRY(data->numedest_entry));
    const gchar *typecont = gtk_entry_get_text(GTK_ENTRY(data->typecont_entry));
    finalizare_plata(widget, data, window);
    
     FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%sconturi.txt", nume);
    // Opening the file in read mode
    file = fopen(filename, "r");
    FILE *tempFile = fopen("temp_file.txt", "w");
    
     if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }
    
    char buffer[100];
    int found = 0;
    int mic = 0;
    long pos;
    int updatedObject;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char stored_type[50],  stored_amount[50];
        if (sscanf(buffer, "%s %s", stored_type, stored_amount) == 2) {
            if (strcmp(typecontd, stored_type) == 0 ) {
                found = 1;
                updatedObject = atoi(stored_amount) + atoi(amount);
                if (updatedObject < 0 ) {
                   mic = 1;
                }
                fprintf(tempFile, "%s %d\n", typecontd, updatedObject);
            }
            else{
            	fprintf(tempFile, "%s %s\n", stored_type, stored_amount);
            }
        }
    }
    file = fopen(filename, "a");
    if (found && !mic) {
       remove(filename); // Delete the original file
        rename("temp_file.txt", filename);
    } else {
        printf("Deposit not found or the sum is too big.\n");
    }
    
     fclose(file);
     finalizare(widget, data, window);
}



void transfer_plati(GtkWidget *widget, EntryWidgets *data, gpointer *window){
      EntryWidgets *entries = (EntryWidgets *)data;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(entries->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Alege contul pentru depunerea banilor");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
   GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);

    GtkWidget *typecont_label = gtk_label_new("Cont:");
    gtk_grid_attach(GTK_GRID(grid), typecont_label, 0, 0, 1, 1);

    GtkWidget *typecont_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), typecont_entry, 1, 0, 1, 1);

    entries->typecont_entry = typecont_entry;
    
    
    GtkWidget *button = gtk_button_new_with_label("Salvare");
    int width = 150;  // Set the desired width
    int height = 150;  // Set the desired height
    
    
    gtk_widget_set_size_request(button, width, height);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(finalizare_plata), entries);
    
    
      gtk_widget_show_all(dialog);
}

void transfer_transfer(GtkWidget *widget, EntryWidgets *data, gpointer *window){
     EntryWidgets *entries = (EntryWidgets *)data;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(entries->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Alege contul pentru transferarea banilor");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
   GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    
    GtkWidget *numedest_label = gtk_label_new("Numele destinatarului:");
    gtk_grid_attach(GTK_GRID(grid), numedest_label, 0, 0, 1, 1);

    GtkWidget *numedest_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), numedest_entry, 1, 0, 1, 1);

    GtkWidget *typecontd_label = gtk_label_new("Contul destinatarului:");
    gtk_grid_attach(GTK_GRID(grid), typecontd_label, 0, 1, 1, 1);

    GtkWidget *typecontd_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), typecontd_entry, 1, 1, 1, 1);
    
    GtkWidget *typecont_label = gtk_label_new("Contul din care virezi banii:");
    gtk_grid_attach(GTK_GRID(grid), typecont_label, 0, 2, 1, 1);

    GtkWidget *typecont_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), typecont_entry, 1, 2, 1, 1);

    entries->typecontd_entry = typecontd_entry;
    entries->numedest_entry = numedest_entry;
    entries->typecont_entry = typecont_entry;
    
    
    GtkWidget *button = gtk_button_new_with_label("Salvare");
    int width = 150;  // Set the desired width
    int height = 150;  // Set the desired height
    
    
    gtk_widget_set_size_request(button, width, height);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(finalizare_transfer), entries);
    
    
      gtk_widget_show_all(dialog);

}


void prelucrare_transfer(GtkWidget *widget, EntryWidgets *data, gpointer *window){
     EntryWidgets *entries = (EntryWidgets *)data;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(entries->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    if(strcmp(type, "deposits") == 0){
    	transfer_depozit(widget, entries, window);
    }
    if(strcmp(type, "withdrawals") == 0){
        transfer_plati(widget, entries, window);
    }
    if(strcmp(type, "transfers") == 0){
    	transfer_transfer(widget, entries, window);
    }
    if(strcmp(type, "payments") == 0){
    	transfer_plati(widget, entries, window);
    }
    

}


void save_transfer(GtkWidget *widget, gpointer date, gpointer *window) {
    EntryWidgets *data = (EntryWidgets *)date;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(data->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(data->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(data->name_entry));

     FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%stransferuri.txt", name);

    // Opening the file in write mode
    file = fopen(filename, "a");
    fprintf(file, "%s %s\n", type, amount);
    fclose(file);
    prelucrare_transfer(widget, data, window);


    
}

void createPersonalFileConturi(const char *name) {
    FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%sconturi.txt", name);
    // Check if the file exists
    if (access(filename, F_OK) == 0) {
        //printf("File '%s' already exists.\n", filename);
        return;
    }

    // Opening the file in write mode
    file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }

    // Closing the file
    fclose(file);
}

void save_cont(GtkWidget *widget, gpointer date, gpointer *window) {
    EntryWidgets *data = (EntryWidgets *)date;
    const gchar *type = gtk_entry_get_text(GTK_ENTRY(data->type_entry));
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(data->amount_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(data->name_entry));

     FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%sconturi.txt", name);

    // Opening the file in write mode
    file = fopen(filename, "a");
    fprintf(file, "%s %s\n", type, amount);
        fclose(file);


    
}


void conturi(GtkWidget *widget, EntryWidgets *data, gpointer *window){

     EntryWidgets *entries = (EntryWidgets *)data;
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entries->id_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries->password_entry));
     createPersonalFileConturi(name);
    
     GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Bine ai venit in contul tau!");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);

    GtkWidget *type_label = gtk_label_new("Type:");
    gtk_grid_attach(GTK_GRID(grid), type_label, 0, 0, 1, 1);

    GtkWidget *type_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), type_entry, 1, 0, 1, 1);

    GtkWidget *amount_label = gtk_label_new("Amount:");
    gtk_grid_attach(GTK_GRID(grid), amount_label, 0, 1, 1, 1);

    GtkWidget *amount_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), amount_entry, 1, 1, 1, 1);

  
    entries->type_entry = type_entry;
    entries->amount_entry = amount_entry;
    
    
    GtkWidget *button = gtk_button_new_with_label("salvare");
    int width = 150;  // Set the desired width
    int height = 150;  // Set the desired height
    
    
    gtk_widget_set_size_request(button, width, height);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(save_cont), entries);
    
    
      gtk_widget_show_all(dialog);
    
}

void tranzactie(GtkWidget *widget, EntryWidgets *data, gpointer *window){
    EntryWidgets *entries = (EntryWidgets *)data;
    
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entries->id_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries->password_entry));
     createPersonalFileTransferuri(name);

    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Efectueaza o noua tranzactie!");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
   GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);

    GtkWidget *type_label = gtk_label_new("Type:");
    gtk_grid_attach(GTK_GRID(grid), type_label, 0, 0, 1, 1);

    GtkWidget *type_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), type_entry, 1, 0, 1, 1);

    GtkWidget *amount_label = gtk_label_new("Amount:");
    gtk_grid_attach(GTK_GRID(grid), amount_label, 0, 1, 1, 1);

    GtkWidget *amount_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), amount_entry, 1, 1, 1, 1);

    entries->type_entry = type_entry;
    entries->amount_entry = amount_entry;
    
    
    GtkWidget *button = gtk_button_new_with_label("Salvare");
    int width = 150;  // Set the desired width
    int height = 150;  // Set the desired height
    
    
    gtk_widget_set_size_request(button, width, height);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(save_transfer), entries);
    
    
      gtk_widget_show_all(dialog);
 
}

void finalizare_balanta(GtkWidget *widget, int sum, gpointer *window){
   GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Balanta contului");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    int my_integer = sum;

    // Convert the integer to a string for display
    gchar *int_str = g_strdup_printf("%d", my_integer);

    // Create a label to display the integer
    GtkWidget *label = gtk_label_new(int_str);
    g_free(int_str);

    // Attach the label to the grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    
    gtk_widget_show_all(dialog);
}

void balanta_contului(GtkWidget *widget, gpointer date, gpointer *window){
	EntryWidgets *data = (EntryWidgets *)date;
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(data->name_entry));
    
     FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%sconturi.txt", name);
    // Opening the file in read mode
    file = fopen(filename, "r");
    
     if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }
    
    char buffer[100];
    int sum = 0;
    int updatedObject;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char stored_type[50],  stored_amount[50];
        if (sscanf(buffer, "%s %s", stored_type, stored_amount) == 2) {
            sum = sum + atoi(stored_amount);
        }
    }
     fclose(file);
     g_print("%d", sum);
     finalizare_balanta(widget, sum, window);
	
}

GtkListStore *create_and_fill_list_store(gpointer data, gpointer *window){
	
     EntryWidgets *entries = (EntryWidgets *)data;
    
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entries->id_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries->password_entry));
    FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%stransferuri.txt", name);

    // Opening the file in write mode
    file = fopen(filename, "r");
  GtkListStore *liststore;
  liststore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
  	
  GtkTreeIter iter;
  char buffer[100];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char stored_type[50],  stored_amount[50];
        if (sscanf(buffer, "%s %s", stored_type, stored_amount) == 2) {
    
    // create the list of columns;
        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter,
 	0, stored_type,
    	1, stored_amount, -1);
    	}
  }
  fclose(file);
  return liststore;
  
  
}

void lista_tranzactii(GtkWidget *widget, gpointer data, gpointer *window){

     EntryWidgets *entries = (EntryWidgets *)data;
    
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entries->id_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries->password_entry));
 
   GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Afisare toate tranzactiile");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    GtkWidget *grid = gtk_grid_new();
    
    gtk_container_add(GTK_CONTAINER(modal_area), grid);
    
   GtkListStore *liststore = create_and_fill_list_store(entries, window);
     GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
     gtk_grid_attach(GTK_GRID(grid), tree_view, 1, 1, 1, 1);
     
     // Create and add columns to the tree view
     GtkCellRenderer *renderer;
     GtkTreeViewColumn *column;

     // First column - Text column
     renderer = gtk_cell_renderer_text_new();
     column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 0, NULL);
     gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
     
     renderer = gtk_cell_renderer_text_new();
     column = gtk_tree_view_column_new_with_attributes("Amount", renderer, "text", 1, NULL);
     gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    
     GtkWidget *l1;
     l1 = gtk_label_new("");
     gtk_grid_attach(GTK_GRID(grid), l1, 1, 2, 1, 1);
     gtk_widget_show_all(dialog);
}

void sterge_cont_finalizare(GtkWidget *widget, gpointer data, gpointer *window){
	EntryWidgets *entries = (EntryWidgets *)data;
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
	const gchar *typecont = gtk_entry_get_text(GTK_ENTRY(entries->typecont_entry));
   
     FILE *file;
    char filename[MAX_NAME_LENGTH + 5]; // Adding space for ".txt" and null terminator

    // Appending ".txt" to the name to create the file name
    snprintf(filename, sizeof(filename), "%sconturi.txt", name);
    // Opening the file in read mode
    file = fopen(filename, "r");
    FILE *tempFile = fopen("temp_file.txt", "w");
    
     if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }
    
    char buffer[100];
    int found = 0;
    int mic = 0;
    long pos;
    int updatedObject;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char stored_type[50],  stored_amount[50];
        if (sscanf(buffer, "%s %s", stored_type, stored_amount) == 2) {
            if (strcmp(typecont, stored_type) == 0 ) {
                found = 1;
            }
            else{
            	fprintf(tempFile, "%s %s\n", stored_type, stored_amount);
            }
        }
    }
    file = fopen(filename, "a");
    if (found && !mic) {
       remove(filename); // Delete the original file
        rename("temp_file.txt", filename);
    } else {
        printf("Deposit not found or the sum is too big.\n");
    }
    
     fclose(file);
     finalizare(widget, data, window);
}

void sterge_cont(GtkWidget *widget, gpointer data, gpointer *window){
     EntryWidgets *entries = (EntryWidgets *)data;
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    
    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Sterge un cont");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
   GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);

    GtkWidget *typecont_label = gtk_label_new("Contul:");
    gtk_grid_attach(GTK_GRID(grid), typecont_label, 0, 0, 1, 1);

    GtkWidget *typecont_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), typecont_entry, 1, 0, 1, 1);


    entries->typecont_entry = typecont_entry;
    
    GtkWidget *button = gtk_button_new_with_label("Salvare");
    
     int width = 150;  // Set the desired width
    int height = 150;  // Set the desired height
    
    gtk_widget_set_size_request(button, width, height);
     gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 1, 1);
     g_signal_connect(button, "clicked", G_CALLBACK(sterge_cont_finalizare), entries);
    
     gtk_widget_show_all(dialog);

}




void login_validated(GtkWidget *widget, EntryWidgets *data, gpointer *window){

    EntryWidgets *entries = (EntryWidgets *)data;
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entries->id_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries->password_entry));

    //the modal area refers to a window or dialog that is "modal," meaning it blocks interaction with other windows in the application until it's closed.
    GtkWidget *dialog=gtk_dialog_new();
    GtkWidget *modal_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), "Bine ai venit in contul tau!");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));// place the table in the middle
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);//principal plan
    gtk_widget_set_size_request(dialog, 1000, 700);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(modal_area), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    
    GtkWidget *button1 = gtk_button_new_with_label("Adaugare cont");
    GtkWidget *button2 = gtk_button_new_with_label("Tranzactie noua");
    GtkWidget *button3 = gtk_button_new_with_label("Balanta contului");
    GtkWidget *button4 = gtk_button_new_with_label("Lista tranzactiilor");
    GtkWidget *button5 = gtk_button_new_with_label("Sterge cont");
    
    int width = 100;  // Set the desired width
    int height = 100;  // Set the desired height
    
    gtk_widget_set_size_request(button1, width, height);
    gtk_widget_set_size_request(button2, width, height);
    gtk_widget_set_size_request(button3, width, height);
    gtk_widget_set_size_request(button4, width, height);
    gtk_widget_set_size_request(button5, width, height);
    
     
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button5, 0, 4, 1, 1);
    

    g_signal_connect(button1, "clicked", G_CALLBACK(conturi), entries);
    g_signal_connect(button2, "clicked", G_CALLBACK(tranzactie), entries);
    g_signal_connect(button3, "clicked", G_CALLBACK(balanta_contului), entries);
    g_signal_connect(button4, "clicked", G_CALLBACK(lista_tranzactii), entries);
    g_signal_connect(button5, "clicked", G_CALLBACK(sterge_cont), entries);
    
    
    gtk_widget_show_all(dialog);
}



// Function to save user data to a file
void save_data(GtkWidget *widget, gpointer data) {
    EntryWidgets *entries = (EntryWidgets *)data;

    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entries->id_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries->password_entry));
    
    FILE *file1 = fopen("users.txt", "r");
    if (file1 == NULL) {
        g_print("Error opening file.\n");
        return;
    }

    char buffer[100];
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file1) != NULL) {
        char stored_id[50], stored_name[50], stored_password[50];
        if (sscanf(buffer, "%s %s %s", stored_id, stored_name, stored_password) == 3) {
            if (strcmp(id, stored_id) == 0 || strcmp(name, stored_name) == 0 && strcmp(password, stored_password) == 0) {
                found = 1;
                break;
            }
        }
    }

    FILE *file = fopen("users.txt", "a");
    if (file != NULL && found == 0) {
        fprintf(file, "%s %s %s\n", id, name, password);
        fclose(file);
    } else {
        g_print("Error opening file for writing.\n");
    }
}

// Function to check login credentials against stored data
void login_data(GtkWidget *widget, gpointer data, gpointer *window) {
    EntryWidgets *entries = (EntryWidgets *)data;

    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entries->id_entry));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries->name_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries->password_entry));

    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        g_print("Error opening file.\n");
        return;
    }

    char buffer[100];
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char stored_id[50], stored_name[50], stored_password[50];
        if (sscanf(buffer, "%s %s %s", stored_id, stored_name, stored_password) == 3) {
            if (strcmp(id, stored_id) == 0 && strcmp(name, stored_name) == 0 && strcmp(password, stored_password) == 0) {
                found = 1;
                break;
            }
        }
    }

    fclose(file);

    if (found) {
        g_print("Login successful!\n");
        login_validated(widget, entries, window);
    } else {
        g_print("Invalid credentials\n");
    }
}




int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Intra in contul tau");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *id_label = gtk_label_new("ID:");
    gtk_grid_attach(GTK_GRID(grid), id_label, 0, 0, 1, 1);

    GtkWidget *id_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);

    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);

    GtkWidget *name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    
    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 2, 1, 1);

    GtkWidget *password_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 2, 1, 1);

    EntryWidgets entries;
    entries.id_entry = id_entry;
    entries.name_entry = name_entry;
    entries.password_entry = password_entry;

    GtkWidget *save_button = gtk_button_new_with_label("Login");
    g_signal_connect(save_button, "clicked", G_CALLBACK(login_data), &entries);
    gtk_grid_attach(GTK_GRID(grid), save_button, 0, 3, 2, 1);

    GtkWidget *load_button = gtk_button_new_with_label("Register");
    g_signal_connect(load_button, "clicked", G_CALLBACK(save_data), &entries);
    gtk_grid_attach(GTK_GRID(grid), load_button, 0, 4, 2, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
