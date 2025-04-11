#include <gtk/gtk.h>
#include <stdio.h>
#define GUI_FILE "mainwin.glade"

int main(int argc, char* argv[]) {

	// Initialize GTK and create a builder
	gtk_init(&argc, &argv);
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// Create the main window, and close on exiting
	GtkWidget* winMain = (GtkWidget*) gtk_builder_get_object(builder, "winMain");
	g_signal_connect(winMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show(winMain);
	
	// Run main loop
	gtk_main();

	// Dereferencing the GTK Builder
	g_object_unref(builder);
	
	// Return;
	return;
	
}
