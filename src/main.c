#include <gtk/gtk.h>
#include <stdio.h>
#include "crud.h"
#include "gtkutils.h"

// Global Variable for Filename
static char filename[512];

// Function to connect to database from the Open Dialog
struct DBConnect_args {
	GtkFileChooser* fileChooseDb;
	GtkWindow* winConnect;
};
void DBConnect(GtkWidget* sender, struct DBConnect_args* args) {
	
	// Make seperate builder
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// Activate spinner
	GtkSpinner* statusBusy = (GtkSpinner*) gtk_builder_get_object(builder, "statusBusy");
	gtk_spinner_start(statusBusy);
	
	// Changing Text
	GtkLabel* statusState = (GtkLabel*) gtk_builder_get_object(builder, "statusState");
	gtk_label_set_text(statusState, "CONNECTING TO DATABASE");
	
	// Setting filename of database
	strcpy(filename, g_file_get_path(gtk_file_chooser_get_file(args->fileChooseDb)));
	
	// Closing the open dialog
	gtk_window_close(args->winConnect);
	
	// Open the database
	if (sqlite3_open(filename, &db_con) != SQLITE_OK) {
		DisplayError(2, "SQLite could not open the database file, please try again another time.");
		gtk_spinner_stop(statusBusy);
		gtk_label_set_text(statusState, "ABORTED");
		return;
	}
	
	// Returning status
	gtk_spinner_stop(statusBusy);
	gtk_label_set_text(statusState, "READY");
}

// Open a new database
void DBOpen(GtkWidget* sender, gpointer no_data) {

	struct DBConnect_args* args = (struct DBConnect_args*) malloc(sizeof(struct DBConnect_args*));
	
	// Make seperate builder
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// Open File Dialog
	GtkWidget* winConnect = (GtkWidget*) gtk_builder_get_object(builder, "winConnect");
	gtk_widget_show(winConnect);
	
	// Open File Chooser
	GtkFileChooser* fileChooseDb = (GtkFileChooser*) gtk_builder_get_object(builder, "fileChooseDb");
	
	// Cancel Button
	GtkWidget* btnConnectCancel = (GtkWidget*) gtk_builder_get_object(builder, "btnConnectCancel");
	g_signal_connect(btnConnectCancel, "clicked", G_CALLBACK(CloseWindow), winConnect);
	
	// Create argument packet
	args->fileChooseDb = fileChooseDb;
	args->winConnect = (GtkWindow*) winConnect;
	
	// Go Button
	GtkWidget* btnConnectGo = (GtkWidget*) gtk_builder_get_object(builder, "btnConnectGo");
	g_signal_connect(btnConnectGo, "clicked", G_CALLBACK(DBConnect), args);
	
	// Remove the extra builder
	g_object_unref(builder);
	
}

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
	
	// Remove the extra builder
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
	
	GtkWidget* mnuOpen = (GtkWidget*) gtk_builder_get_object(builder, "mnuOpen");
	g_signal_connect(mnuOpen, "activate", G_CALLBACK(DBOpen), NULL);
	
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
