#include <gtk/gtk.h>
#include "sqlite3.h"
#define GUI_FILE "mainwin.glade"

// Function to close window on button click
void CloseWindow(GtkWidget* sender, GtkWindow* win) {gtk_window_close(win);}

// Error generator
void DisplayError(int level, const char* message) {

	char* icon_name;
	
	// Make seperate builder
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// About Box Dialog
	GtkWidget* winError = (GtkWidget*) gtk_builder_get_object(builder, "winError");
	gtk_widget_show(winError);
	
	// Message Label
	GtkLabel* lblMessage = (GtkLabel*) gtk_builder_get_object(builder, "lblMessage");
	gtk_label_set_text(lblMessage, message);
	
	GtkImage* imgError = (GtkImage*) gtk_builder_get_object(builder, "imgError");
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
	gtk_image_set_from_icon_name(imgError, icon_name, GTK_ICON_SIZE_DIALOG);
	
	// Ok Button
	GtkWidget* btnErrorOk = (GtkWidget*) gtk_builder_get_object(builder, "btnErrorOk");
	g_signal_connect(btnErrorOk, "clicked", G_CALLBACK(CloseWindow), winError);
	
	// Remove the extra builder
	g_object_unref(builder);
	
}
