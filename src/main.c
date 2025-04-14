#include <gtk/gtk.h>
#include <stdio.h>
#define GUI_FILE "mainwin.glade"

// Function to close window on button click
void CloseWindow(GtkWidget* sender, GtkWindow* win) {gtk_window_close(win);}

// Show About Box
void ShowAbout(GtkWidget* sender, gpointer no_data) {

	// Make seperate builder
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// About Box Dialog
	GtkWidget* winAbout = (GtkWidget*) gtk_builder_get_object(builder, "winAbout");
	gtk_widget_show(winAbout);
	
	// Ok Button
	GtkWidget* btnAboutOk = (GtkWidget*) gtk_builder_get_object(builder, "btnAboutOk");
	g_signal_connect(btnAboutOk, "clicked", G_CALLBACK(CloseWindow), winAbout);
	
	g_object_unref(builder);
}

// Main program
int main(int argc, char* argv[]) {

    /* -- GTK Initialization -- */
    
	// Initialize GTK and create a builder
	gtk_init(&argc, &argv);
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	/* -- Main Window -- */
	
	// Create the main window, and close on exiting
	GtkWidget* winMain = (GtkWidget*) gtk_builder_get_object(builder, "winMain");
	g_signal_connect(winMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show(winMain);
	
	/* -- File Menu Items -- */
	
	// TODO: Open
	// TODO: Save
	
	// Exit Menu Item
	GtkWidget* mnuQuit = (GtkWidget*) gtk_builder_get_object(builder, "mnuQuit");
	g_signal_connect(mnuQuit, "activate", G_CALLBACK(gtk_main_quit), NULL);
	
	/* -- Help Menu Items -- */
	
	// Display About
	GtkWidget* mnuAbout = (GtkWidget*) gtk_builder_get_object(builder, "mnuAbout");
	g_signal_connect(mnuAbout, "activate", G_CALLBACK(ShowAbout), NULL);
	
	
	/* -- GTK Main Loop -- */
	
	// Run main loop
	gtk_main();

    /* -- GTK Dereferencing -- */
    
	// Dereferencing the GTK Builder
	g_object_unref(builder);
	
	/* -- Exit Program -- */
	
	// Return;
	return 0;
	
}
