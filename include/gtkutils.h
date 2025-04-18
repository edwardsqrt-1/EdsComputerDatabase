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
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(buff, &start, &end); 

	// Delete everything
	gtk_text_buffer_delete(buff, &start, &end);
  
}

void DBAdd(GtkWidget* sender, gpointer no_data) {
	DisplayError(0, "Add");
}

void DBUpdate(GtkWidget* sender, gpointer no_data) {
	DisplayError(0, "Update");
}

void DBDelete(GtkWidget* sender, gpointer no_data) {
	DisplayError(0, "Delete");
}

struct ParseCustom_args {
	GtkTextView* txtCustom;
	GtkWindow* winCustom;
};
void ParseCustom(GtkWidget* sender, struct ParseCustom_args* args) {
	
	// Get text buffer and bounds
	GtkTextBuffer* buff = gtk_text_view_get_buffer(args->txtCustom);
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(buff, &start, &end); 

	// Get text from text buffer
	char* command = (char*) gtk_text_buffer_get_text(buff, &start, &end, FALSE);
	
	// Close Window
	gtk_window_close(args->winCustom);
	
	// Execute command in sql.h
	if (runSQL(command) == 0) {
		DisplayError(0, "Command was executed successfully!");
	} else {
		DisplayError(2, "An error was generated!");
	}
}

void DBCustom(GtkWidget* sender, gpointer no_data) {
	
	// Make seperate builder
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// Custom Box Dialog
	GtkWidget* winCustom = (GtkWidget*) gtk_builder_get_object(builder, "winCustom");
	gtk_widget_show(winCustom);
	
	// Cancel Button
	GtkTextView* txtCustom = (GtkTextView*) gtk_builder_get_object(builder, "txtCustom");
	
	// Cancel Button
	GtkWidget* btnCustomCancel = (GtkWidget*) gtk_builder_get_object(builder, "btnCustomCancel");
	g_signal_connect(btnCustomCancel, "clicked", G_CALLBACK(CloseWindow), winCustom);
	
	// Create argument packet
	struct ParseCustom_args* args = (struct ParseCustom_args*) malloc(sizeof(struct ParseCustom_args));
	args->txtCustom = txtCustom;
	args->winCustom = (GtkWindow*) winCustom;
	
	// Go Button
	GtkWidget* btnCustomGo = (GtkWidget*) gtk_builder_get_object(builder, "btnCustomGo");
	g_signal_connect(btnCustomGo, "clicked", G_CALLBACK(ParseCustom), args);
}
