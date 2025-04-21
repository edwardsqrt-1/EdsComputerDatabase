#include <gtk/gtk.h>
#include <stdio.h>
#include "sql.h"
#include "gtkutils.h"

// Function to connect to database from the Open Dialog
struct DBConnect_args {
	GtkFileChooser* fileChooseDb;
	GtkWindow* winConnect;
	GtkSpinner* statusBusy;
	GtkLabel* statusState;
	GtkLabel* statusFile;
};
void DBConnect(GtkWidget* sender, struct DBConnect_args* args) {

	// Make seperate builder
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);
	
	// Activate spinner
	gtk_spinner_start(args->statusBusy);
	
	// Changing Text
	gtk_label_set_text(args->statusState, "CONNECTING");
	
	// Setting filename of database
	strcpy(filename, g_file_get_path(gtk_file_chooser_get_file(args->fileChooseDb)));
	
	// Closing the open dialog
	gtk_window_close(args->winConnect);
	
	// Open the database
	if (sqlite3_open(filename, &db_con) != SQLITE_OK) {
		DisplayError(2, "SQLite could not open the database file, please try again another time.\n");
		gtk_spinner_stop(args->statusBusy);
		gtk_label_set_text(args->statusState, "ABORTED");
		strcpy(filename, "\0");
		return;
	}

	// Show open database in the file status
	gtk_label_set_text(args->statusFile, filename);

	// Returning status
	DisplayError(0, "SQLite successfully opened the file!\n");
	gtk_spinner_stop(args->statusBusy);
	gtk_label_set_text(args->statusState, "READY");
	
}

// Open a new database
void DBOpen(GtkWidget* sender, struct DBConnect_args* args) {
	
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
	
	// Add to argument packet
	args->fileChooseDb = fileChooseDb;
	args->winConnect = (GtkWindow*) winConnect;
	
	// Go Button
	GtkWidget* btnConnectGo = (GtkWidget*) gtk_builder_get_object(builder, "btnConnectGo");
	g_signal_connect(btnConnectGo, "clicked", G_CALLBACK(DBConnect), args);
	
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

	/* -- Text Box -- */
	
	GtkWidget* txtView = (GtkWidget*) gtk_builder_get_object(builder, "txtView");

	/* -- Text Search Box -- */
	
	GtkWidget* txtSearch = (GtkWidget*) gtk_builder_get_object(builder, "txtSearch");
	g_signal_connect(txtSearch, "changed", G_CALLBACK(ShowWFilter), txtView);

	/* -- Status Items -- */

	// Loading circle 
	GtkWidget* statusBusy = (GtkWidget*) gtk_builder_get_object(builder, "statusBusy");

	// Status indicator in words
	GtkWidget* statusState = (GtkWidget*) gtk_builder_get_object(builder, "statusState");

	// Displays filename
	GtkWidget* statusFile = (GtkWidget*) gtk_builder_get_object(builder, "statusFile");
	
	/* -- File Menu Items -- */
	
	// Open Menu Item
	struct DBConnect_args open_args;
	open_args.statusBusy = (GtkSpinner*) statusBusy;
	open_args.statusState = (GtkLabel*) statusState;
	open_args.statusFile = (GtkLabel*) statusFile;
	GtkWidget* mnuOpen = (GtkWidget*) gtk_builder_get_object(builder, "mnuOpen");
	g_signal_connect(mnuOpen, "activate", G_CALLBACK(DBOpen), &open_args);
	
	// Exit Menu Item
	GtkWidget* mnuQuit = (GtkWidget*) gtk_builder_get_object(builder, "mnuQuit");
	g_signal_connect(mnuQuit, "activate", G_CALLBACK(gtk_main_quit), NULL);

	/* -- View Menu Items -- */

	struct DBDump_args view_args;
	view_args.txtDisplay = (GtkTextView*) txtView;
	view_args.statusBusy = (GtkSpinner*) statusBusy;
	view_args.statusState = (GtkLabel*) statusState;

	// Show Desktops Item
	GtkWidget* mnuDesktops = (GtkWidget*) gtk_builder_get_object(builder, "mnuDesktops");
	g_signal_connect(mnuDesktops, "activate", G_CALLBACK(DBDump), &view_args);

	// Show Laptops Item
	GtkWidget* mnuLaptops = (GtkWidget*) gtk_builder_get_object(builder, "mnuLaptops");
	g_signal_connect(mnuLaptops, "activate", G_CALLBACK(DBDump), &view_args);

	// Show Monitors Item
	GtkWidget* mnuMonitors = (GtkWidget*) gtk_builder_get_object(builder, "mnuMonitors");
	g_signal_connect(mnuMonitors, "activate", G_CALLBACK(DBDump), &view_args);

	// Show Tablets Item
	GtkWidget* mnuTablets = (GtkWidget*) gtk_builder_get_object(builder, "mnuTablets");
	g_signal_connect(mnuTablets, "activate", G_CALLBACK(DBDump), &view_args);

	// Show Phones Item
	GtkWidget* mnuPhones = (GtkWidget*) gtk_builder_get_object(builder, "mnuPhones");
	g_signal_connect(mnuPhones, "activate", G_CALLBACK(DBDump), &view_args);

	// Show Statuses Item
	GtkWidget* mnuStatus = (GtkWidget*) gtk_builder_get_object(builder, "mnuStatus");
	g_signal_connect(mnuStatus, "activate", G_CALLBACK(DBDump), &view_args);
	
	/* -- Operation Menu Items -- */
	
	// Create Record Item
	struct ParseCRUD_args add_args;
	add_args.statusBusy = (GtkSpinner*) statusBusy;
	add_args.statusState = (GtkLabel*) statusState;
	add_args.txtDisplay = (GtkTextView*) txtView;
	GtkWidget* mnuAdd = (GtkWidget*) gtk_builder_get_object(builder, "mnuAdd");
	g_signal_connect(mnuAdd, "activate", G_CALLBACK(DBAdd), &add_args);
	
	// Update Record Item
	struct ParseCRUD_args update_args;
	update_args.statusBusy = (GtkSpinner*) statusBusy;
	update_args.statusState = (GtkLabel*) statusState;
	update_args.txtDisplay = (GtkTextView*) txtView;
	GtkWidget* mnuUpdate = (GtkWidget*) gtk_builder_get_object(builder, "mnuUpdate");
	g_signal_connect(mnuUpdate, "activate", G_CALLBACK(DBUpdate), &update_args);
	
	// Delete Record Item
	struct ParseCRUD_args delete_args;
	delete_args.statusBusy = (GtkSpinner*) statusBusy;
	delete_args.statusState = (GtkLabel*) statusState;
	delete_args.txtDisplay = (GtkTextView*) txtView;
	GtkWidget* mnuDelete = (GtkWidget*) gtk_builder_get_object(builder, "mnuDelete");
	g_signal_connect(mnuDelete, "activate", G_CALLBACK(DBDelete), &delete_args);
	
	// Custom SQL Item
	struct ParseCustom_args custom_args;
	custom_args.statusBusy = (GtkSpinner*) statusBusy;
	custom_args.statusState = (GtkLabel*) statusState;
	custom_args.txtDisplay = (GtkTextView*) txtView;
	GtkWidget* mnuCustom = (GtkWidget*) gtk_builder_get_object(builder, "mnuCustom");
	g_signal_connect(mnuCustom, "activate", G_CALLBACK(DBCustom), &custom_args);
	
	/* -- Help Menu Items -- */
	
	// Display About
	GtkWidget* mnuAbout = (GtkWidget*) gtk_builder_get_object(builder, "mnuAbout");
	g_signal_connect(mnuAbout, "activate", G_CALLBACK(ShowAbout), NULL);
	
	/* -- Button Row Items -- */
	
	// Button to filter information
	GtkWidget* btnFilter = (GtkWidget*) gtk_builder_get_object(builder, "btnFilter");
	//g_signal_connect(btnFilter, "activate", G_CALLBACK(DBDelete), NULL);
	
	// Button to clear viewer
	GtkWidget* btnClear = (GtkWidget*) gtk_builder_get_object(builder, "btnClear");
	g_signal_connect(btnClear, "clicked", G_CALLBACK(ClearView), txtView);
	
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
