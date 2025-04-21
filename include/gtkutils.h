#include <gtk/gtk.h>
#include "sqlite3.h"
#define GUI_FILE "mainwin.glade"

// Function to close window on button click
void CloseWindow(GtkWidget* sender, GtkWindow* win) {gtk_window_close(win);}

// Error generator
void DisplayError(int level, const char* message) {

	// Variable to store a stock icon name
	char* icon_name;
	
	// Make seperate builder
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// About Box Dialog
	GtkWidget* winError = (GtkWidget*) gtk_builder_get_object(builder, "winError");
	gtk_widget_show(winError);
	
	// Message Label
	GtkLabel* lblMessage = (GtkLabel*) gtk_builder_get_object(builder, "lblMessage");
	gtk_label_set_text(lblMessage, message);
	
	// Image for Error Dialog
	GtkImage* imgError = (GtkImage*) gtk_builder_get_object(builder, "imgError");
	
	// Choose image based on level of error
	switch (level) {
	
		case 0:
			icon_name = "dialog-information";
			break;
		case 1:
			icon_name = "dialog-warning";
			break;
		case 2:
			icon_name = "dialog-error";
			break;
		default:
			icon_name = "edit-delete";
		
	}
	
	// Set Image
	gtk_image_set_from_icon_name(imgError, icon_name, GTK_ICON_SIZE_DIALOG);
	
	// Ok Button
	GtkWidget* btnErrorOk = (GtkWidget*) gtk_builder_get_object(builder, "btnErrorOk");
	g_signal_connect(btnErrorOk, "clicked", G_CALLBACK(CloseWindow), winError);
	
	// Remove the extra builder
	g_object_unref(builder);
	
}

// This function clears the text in the display window
void ClearView(GtkWidget* sender, GtkTextView* txtView) {

 	// Get text buffer and bounds
	GtkTextBuffer* buff = gtk_text_view_get_buffer(txtView); 
	gtk_text_buffer_set_text(buff, "", 0);
	gtk_text_view_set_buffer(txtView, buff);
	/*GtkTextIter start, end;
	gtk_text_buffer_get_bounds(buff, &start, &end); 

	// Delete everything
	gtk_text_buffer_delete(buff, &start, &end);*/
  
}

void ShowWFilter (GtkEntry* txtSearch, GtkTextView* txtView) {
	char clause[300];

	sprintf(clause, "WHERE Model LIKE '%%%s%%';", gtk_entry_get_text(txtSearch));

	char command[1024];
	strcpy(command, "SELECT Maker, Model, ID FROM (SELECT Maker, Model, ID FROM Desktops UNION ALL SELECT Maker, Model, ID FROM Laptops UNION ALL SELECT Maker, Model, ID FROM Monitors UNION ALL SELECT Maker, Model, ID FROM Tablets UNION ALL SELECT Maker, Model, ID FROM Phones) ");
	strcat(command, clause);

	// Execute command in sql.h
	if (runSQL(command) != 0) return;

	// Show output
	GtkTextBuffer* buff = gtk_text_view_get_buffer(txtView); 
	gtk_text_buffer_set_text(buff, output, strlen(output));
	gtk_text_view_set_buffer(txtView, buff);
	strcpy(output, "\0");

}

// Function to add a database item
void DBAdd(GtkWidget* sender, gpointer no_data) {
	DisplayError(0, "Add");
}

// Function to update a database item
void DBUpdate(GtkWidget* sender, gpointer no_data) {
	DisplayError(0, "Update");
}

// Function to delete a database item
void DBDelete(GtkWidget* sender, gpointer no_data) {
	DisplayError(0, "Delete");
}

// Function to parse the custom SQL statement and send it to database
struct ParseCustom_args {
	GtkTextView* txtCustom;
	GtkWindow* winCustom;
	GtkTextView* txtDisplay;
	GtkSpinner* statusBusy;
	GtkLabel* statusState;
};
void ParseCustom(GtkWidget* sender, struct ParseCustom_args* args) {
	
	// Get text buffer and bounds; delete current contents
	GtkTextBuffer* buff = gtk_text_view_get_buffer(args->txtCustom);
	GtkTextIter c_start, c_end, t_start, t_end;
  	gtk_text_buffer_get_bounds(buff, &c_start, &c_end); 

	// Get text from text buffer
	char* command = (char*) gtk_text_buffer_get_text(buff, &c_start, &c_end, FALSE);
	
	// Close Window
	gtk_window_close(args->winCustom);
	
	// Show status as sending command
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "SENDING COMMAND");

	// Execute command in sql.h
	if (runSQL(command) == 0) {
		DisplayError(0, "Command was executed successfully!");
		gtk_label_set_text(args->statusState, "COMMAND SUCCESS");
	} else {
		DisplayError(2, "An error was generated!");
		gtk_label_set_text(args->statusState, "COMMAND FAIL");
	}

	// Stop spinner either way
	gtk_spinner_stop(args->statusBusy);

	// Show output
	GtkTextBuffer* buff2 = gtk_text_view_get_buffer(args->txtDisplay); 
	gtk_text_buffer_set_text(buff2, output, strlen(output));
	gtk_text_view_set_buffer(args->txtDisplay, buff2);
	strcpy(output, "\0");
	
}

// Creates a window prompting the user to make an SQL Command
void DBCustom(GtkWidget* sender, struct ParseCustom_args* args) {
	
	// Make seperate builder
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// Custom Box Dialog
	GtkWidget* winCustom = (GtkWidget*) gtk_builder_get_object(builder, "winCustom");
	gtk_widget_show(winCustom);
	
	// Custom SQL textbox
	GtkTextView* txtCustom = (GtkTextView*) gtk_builder_get_object(builder, "txtCustom");
	
	// Cancel Button
	GtkWidget* btnCustomCancel = (GtkWidget*) gtk_builder_get_object(builder, "btnCustomCancel");
	g_signal_connect(btnCustomCancel, "clicked", G_CALLBACK(CloseWindow), winCustom);
	
	// Create argument packet
	args->txtCustom = txtCustom;
	args->winCustom = (GtkWindow*) winCustom;

	// Status showing waiting for input
	gtk_label_set_text(args->statusState, "WAITING FOR USER INPUT");
	
	// Go Button
	GtkWidget* btnCustomGo = (GtkWidget*) gtk_builder_get_object(builder, "btnCustomGo");
	g_signal_connect(btnCustomGo, "clicked", G_CALLBACK(ParseCustom), args);
}
