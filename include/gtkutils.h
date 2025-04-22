#include <gtk/gtk.h>
#include <stdio.h>
#include "sqlite3.h"
#define GUI_FILE "mainwin.glade"

// Define a struct to handle CRUD Operations
struct ParseCRUD_args {
	GtkBuilder* builder;
	GtkWindow* winCRUD;
	GtkTextView* txtDisplay;
	GtkSpinner* statusBusy;
	GtkLabel* statusState;
};

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
	
		case 0: // Note
			icon_name = "dialog-information";
			break;
		case 1: // Warning (non-fatal)
			icon_name = "dialog-warning";
			break;
		case 2: // Error (fatal)
			icon_name = "dialog-error";
			break;
		default: // Unknown
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

 	// Replace text buffer with an empty one
	GtkTextBuffer* buff = gtk_text_view_get_buffer(txtView); 
	gtk_text_buffer_set_text(buff, "", 0);
	gtk_text_view_set_buffer(txtView, buff);
  
}

// Function to dynamically search for a model number across all tables
void ShowWFilter (GtkEntry* txtSearch, GtkTextView* txtView) {
	
	// Declare string arrays necessary to send SQL to the database
	char clause[300];
	char query[1024];

	// Create the clause, query, and combine the two into the query
	sprintf(clause, "WHERE Model LIKE '%%%s%%';", gtk_entry_get_text(txtSearch));
	strcpy(query, "SELECT Maker, Model, ID, Remarks FROM (SELECT Maker, Model, ID, Remarks FROM Desktops UNION ALL SELECT Maker, Model, ID, Remarks FROM Laptops UNION ALL SELECT Maker, Model, ID, Remarks FROM Monitors UNION ALL SELECT Maker, Model, ID, Remarks FROM Tablets UNION ALL SELECT Maker, Model, ID, Remarks FROM Phones) ");
	strcat(query, clause);

	// Execute command in sql.h
	if (runSQL(query) != 0) return;

	// Show and clean output buffer
	GtkTextBuffer* buff = gtk_text_view_get_buffer(txtView); 
	gtk_text_buffer_set_text(buff, output, strlen(output));
	gtk_text_view_set_buffer(txtView, buff);
	strcpy(output, "\0");

}

// Function to parse addition of items to database
void ParseAdd(GtkWidget* sender, struct ParseCRUD_args *args) {

	// Array that contains the query
	char query[1024];

	// Set Status Indicator to parsing options
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "PARSING OPTIONS");

	// Get Notebook object to determine what page is used
	GtkNotebook* ntbInsert = (GtkNotebook*) gtk_builder_get_object(args->builder, "ntbInsert");

	// Find current page and create SQL Query
	switch (gtk_notebook_get_current_page(ntbInsert)) {

		// Desktops
		case 0: { 
			
			// Desktop Addition Tools
			GtkEntry* txtAddDesktopMaker = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopMaker");
			GtkEntry* txtAddDesktopModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopModel");
			GtkSpinButton* numAddDesktopID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddDesktopID");
			GtkEntry* txtAddDesktopCPU = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopCPU");
			GtkEntry* txtAddDesktopGraphics = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopGraphics");
			GtkEntry* txtAddDesktopRAM = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopRAM");
			GtkEntry* txtAddDesktopStorage = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopStorage");
			GtkEntry* txtAddDesktopNetwork = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopNetwork");
			GtkEntry* txtAddDesktopAudio = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopAudio");
			GtkEntry* txtAddDesktopDisplay = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopDisplay");
			GtkEntry* txtAddDesktopOS = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopOS");
			GtkSpinButton* numAddDesktopStatus = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddDesktopStatus");
			GtkEntry* txtAddDesktopComment = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddDesktopComment");
			
			// Create variables to store information in
			char maker[50], model[50], cpu[50], graphics[50], ram[50], storage[50], net[50], audio[50], disp[50], os[50], com[50];
			int id, status;

			// Extract results (strings)
			strcpy(maker, gtk_entry_get_text(txtAddDesktopMaker)); strcpy(model, gtk_entry_get_text(txtAddDesktopModel));
			strcpy(cpu, gtk_entry_get_text(txtAddDesktopCPU)); strcpy(graphics, gtk_entry_get_text(txtAddDesktopGraphics)); 
			strcpy(ram, gtk_entry_get_text(txtAddDesktopRAM)); strcpy(storage, gtk_entry_get_text(txtAddDesktopStorage)); 
			strcpy(net, gtk_entry_get_text(txtAddDesktopNetwork)); strcpy(audio, gtk_entry_get_text(txtAddDesktopAudio)); 
			strcpy(disp, gtk_entry_get_text(txtAddDesktopDisplay)); strcpy(os, gtk_entry_get_text(txtAddDesktopOS)); 
			strcpy(com, gtk_entry_get_text(txtAddDesktopComment)); 

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numAddDesktopID); status = gtk_spin_button_get_value_as_int(numAddDesktopStatus);
			
			// Construct query and break
			sprintf(query, "INSERT INTO Desktops VALUES ('%s', '%s', %u, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s');",
				maker, model, id, cpu, graphics, ram, storage, net, audio, disp, os, status, com);
			break;

		}

		// Laptops
		case 1: {

			// Laptop Addition Tools
			GtkEntry* txtAddLaptopMaker = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopMaker");
			GtkEntry* txtAddLaptopModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopModel");
			GtkSpinButton* numAddLaptopID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddLaptopID");
			GtkEntry* txtAddLaptopCPU = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopCPU");
			GtkEntry* txtAddLaptopGPU = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopGPU");
			GtkEntry* txtAddLaptopRAM = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopRAM");
			GtkEntry* txtAddLaptopStorage = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopStorage");
			GtkEntry* txtAddLaptopNetwork = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopNetwork");
			GtkEntry* txtAddLaptopAudio = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopAudio");
			GtkEntry* txtAddLaptopScreen = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopScreen");
			GtkEntry* txtAddLaptopBattery = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopBattery");
			GtkEntry* txtAddLaptopOS = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopOS");
			GtkSpinButton* numAddLaptopStatus = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddLaptopStatus");
			GtkEntry* txtAddLaptopComment = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddLaptopComment");

			// Create variables to store information in
			char maker[50], model[50], cpu[50], gpu[50], ram[50], storage[50], net[50], audio[50], scrn[50], batt[50], os[50], com[50];
			int id, status;

			// Extract results (strings)
			strcpy(maker, gtk_entry_get_text(txtAddLaptopMaker)); strcpy(model, gtk_entry_get_text(txtAddLaptopModel));
			strcpy(cpu, gtk_entry_get_text(txtAddLaptopCPU)); strcpy(gpu, gtk_entry_get_text(txtAddLaptopGPU)); 
			strcpy(ram, gtk_entry_get_text(txtAddLaptopRAM)); strcpy(storage, gtk_entry_get_text(txtAddLaptopStorage)); 
			strcpy(net, gtk_entry_get_text(txtAddLaptopNetwork)); strcpy(audio, gtk_entry_get_text(txtAddLaptopAudio)); 
			strcpy(scrn, gtk_entry_get_text(txtAddLaptopScreen)); strcpy(batt, gtk_entry_get_text(txtAddLaptopBattery));
			strcpy(os, gtk_entry_get_text(txtAddLaptopOS)); strcpy(com, gtk_entry_get_text(txtAddLaptopComment)); 

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numAddLaptopID); status = gtk_spin_button_get_value_as_int(numAddLaptopStatus);
			
			// Construct query and break
			sprintf(query, "INSERT INTO Laptops VALUES ('%s', '%s', %u, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s');",
				maker, model, id, cpu, gpu, ram, storage, net, audio, scrn, batt, os, status, com);
			break;

		}
		
		// Monitors
		case 2: {

			// Monitor Addition Tools
			GtkEntry* txtAddMonitorMaker = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddMonitorMaker");
			GtkEntry* txtAddMonitorModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddMonitorModel");
			GtkSpinButton* numAddMonitorID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddMonitorID");
			GtkEntry* txtAddMonitorScreen = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddMonitorScreen");
			GtkSpinButton* numAddMonitorStatus = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddMonitorStatus");
			GtkEntry* txtAddMonitorComment = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddMonitorComment");

			// Create variables to store information in
			char maker[50], model[50], scrn[50], com[50];
			int id, status;

			// Extract results (strings)
			strcpy(maker, gtk_entry_get_text(txtAddMonitorMaker)); strcpy(model, gtk_entry_get_text(txtAddMonitorModel));
			strcpy(scrn, gtk_entry_get_text(txtAddMonitorScreen)); strcpy(com, gtk_entry_get_text(txtAddMonitorComment)); 

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numAddMonitorID); status = gtk_spin_button_get_value_as_int(numAddMonitorStatus);
			
			// Construct query and break
			sprintf(query, "INSERT INTO Monitors VALUES ('%s', '%s', %u, '%s', %d, '%s');",
				maker, model, id, scrn, status, com);
			break;

		}
			
		// Tablets
		case 3: {

			// Tablet Addition Tools
			GtkEntry* txtAddTabletMaker = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddTabletMaker");
			GtkEntry* txtAddTabletModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddTabletModel");
			GtkSpinButton* numAddTabletID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddTabletID");
			GtkEntry* txtAddTabletCPU = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddTabletCPU");
			GtkEntry* txtAddTabletRAM = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddTabletRAM");
			GtkEntry* txtAddTabletStorage = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddTabletStorage");
			GtkEntry* txtAddTabletOS = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddTabletOS");
			GtkSpinButton* numAddTabletStatus = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddTabletStatus");
			GtkEntry* txtAddTabletComment = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddTabletComment");

			// Create variables to store information in
			char maker[50], model[50], cpu[50], ram[50], storage[50], os[50], com[50];
			int id, status;

			// Extract results (strings)
			strcpy(maker, gtk_entry_get_text(txtAddTabletMaker)); strcpy(model, gtk_entry_get_text(txtAddTabletModel));
			strcpy(cpu, gtk_entry_get_text(txtAddTabletCPU)); strcpy(ram, gtk_entry_get_text(txtAddTabletRAM)); 
			strcpy(storage, gtk_entry_get_text(txtAddTabletStorage)); strcpy(os, gtk_entry_get_text(txtAddTabletOS)); 
			strcpy(com, gtk_entry_get_text(txtAddTabletComment)); 

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numAddTabletID); status = gtk_spin_button_get_value_as_int(numAddTabletStatus);
			
			// Construct query and break
			sprintf(query, "INSERT INTO Tablets VALUES ('%s', '%s', %u, '%s', '%s', '%s', '%s', %d, '%s');",
				maker, model, id, cpu, ram, storage, os, status, com);
			break;

		}
			
		// Phones
		case 4: {

			// Phone Addition Tools
			GtkEntry* txtAddPhoneMaker = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddPhoneMaker");
			GtkEntry* txtAddPhoneModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddPhoneModel");
			GtkSpinButton* numAddPhoneID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddPhoneID");
			GtkEntry* txtAddPhoneCPU = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddPhoneCPU");
			GtkEntry* txtAddPhoneRAM = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddPhoneRAM");
			GtkEntry* txtAddPhoneStorage = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddPhoneStorage");
			GtkEntry* txtAddPhoneOS = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddPhoneOS");
			GtkSpinButton* numAddPhoneStatus = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddPhoneStatus");
			GtkEntry* txtAddPhoneComment = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddPhoneComment");

			// Create variables to store information in
			char maker[50], model[50], cpu[50], ram[50], storage[50], os[50], com[50];
			int id, status;

			// Extract results (strings)
			strcpy(maker, gtk_entry_get_text(txtAddPhoneMaker)); strcpy(model, gtk_entry_get_text(txtAddPhoneModel));
			strcpy(cpu, gtk_entry_get_text(txtAddPhoneCPU)); strcpy(ram, gtk_entry_get_text(txtAddPhoneRAM)); 
			strcpy(storage, gtk_entry_get_text(txtAddPhoneStorage)); strcpy(os, gtk_entry_get_text(txtAddPhoneOS)); 
			strcpy(com, gtk_entry_get_text(txtAddPhoneComment)); 

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numAddPhoneID); status = gtk_spin_button_get_value_as_int(numAddPhoneStatus);
			
			// Construct query and break
			sprintf(query, "INSERT INTO Phones VALUES ('%s', '%s', %u, '%s', '%s', '%s', '%s', %d, '%s');",
				maker, model, id, cpu, ram, storage, os, status, com);
			break;

		}
			
		// Status
		case 5: {

			// Status Addition Tools
			GtkSpinButton* numAddStatusID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddStatusID");
			GtkEntry* txtAddStatusDescript = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddStatusDescript");
			GtkSpinButton* numAddStatusPriority = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numAddStatusPriority");
			GtkEntry* txtAddStatusComment = (GtkEntry*) gtk_builder_get_object(args->builder, "txtAddStatusComment");

			// Create variables to store information in
			char descript[50], com[50];
			int id, priority;

			// Extract results (strings)
			strcpy(descript, gtk_entry_get_text(txtAddStatusDescript)); 
			strcpy(com, gtk_entry_get_text(txtAddStatusComment));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numAddStatusID); 
			priority = gtk_spin_button_get_value_as_int(numAddStatusPriority);
			
			// Construct query and break
			sprintf(query, "INSERT INTO Status VALUES (%d, '%s', %d, '%s');", id, descript, priority, com);
			break;

		}

		// Error
		default: {

			// Display an error and return
			DisplayError(2, "Invalid page selected!");
			return;

		}	

	}

	// Set Status Indicator to sending command; also close the Insert Window
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "SENDING COMMAND");
	CloseWindow(NULL, args->winCRUD);

	// Execute command in sql.h
	if (runSQL(query) == 0) {
		DisplayError(0, "Command was executed successfully!");
		gtk_label_set_text(args->statusState, "COMMAND SUCCESS");
	} else {
		DisplayError(2, "An error was generated!");
		gtk_label_set_text(args->statusState, "COMMAND FAIL");
	}

	// Stop spinner 
	gtk_spinner_stop(args->statusBusy);

	// Show output
	GtkTextBuffer* buff2 = gtk_text_view_get_buffer(args->txtDisplay); 
	gtk_text_buffer_set_text(buff2, output, strlen(output));
	gtk_text_view_set_buffer(args->txtDisplay, buff2);
	strcpy(output, "\0");
	
}

// Function to add a database item
void DBAdd(GtkWidget* sender, struct ParseCRUD_args *args) {
	
	// Create a new builder to show dialog box
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);

	// Get Insert Dialog
	GtkWidget* winInsert = (GtkWidget*) gtk_builder_get_object(builder, "winInsert");
	gtk_widget_show(winInsert);
	args->winCRUD = (GtkWindow*) winInsert;
	args->builder = builder;

	// Cancel Button
	GtkWidget* btnInsertCancel = (GtkWidget*) gtk_builder_get_object(builder, "btnInsertCancel");
	g_signal_connect(btnInsertCancel, "clicked", G_CALLBACK(CloseWindow), winInsert);

	// Go Button
	GtkWidget* btnInsertGo = (GtkWidget*) gtk_builder_get_object(builder, "btnInsertGo");
	g_signal_connect(btnInsertGo, "clicked", G_CALLBACK(ParseAdd), args);

	// Status showing waiting for input
	gtk_label_set_text(args->statusState, "WAITING FOR USER INPUT");

}

// Function to parse updating of items to database
void ParseUpdate(GtkWidget* sender, struct ParseCRUD_args *args) {

	// Array that contains the query
	char query[1024];

	// Set Status Indicator to parsing options
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "PARSING OPTIONS");

	// Get Notebook object to determine what page is used
	GtkNotebook* ntbUpdate = (GtkNotebook*) gtk_builder_get_object(args->builder, "ntbUpdate");

	// Find current page and create SQL Query
	switch (gtk_notebook_get_current_page(ntbUpdate)) {

		// Desktops
		case 0: { 
			
			// Desktop Update Tools
			GtkEntry* txtUpdateDesktopModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateDesktopModel");
			GtkSpinButton* numUpdateDesktopID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numUpdateDesktopID");
			GtkComboBox* cmbUpdateDesktopCol = (GtkComboBox*) gtk_builder_get_object(args->builder, "cmbUpdateDesktopCol");
			GtkEntry* txtUpdateDesktopVal = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateDesktopVal");
			
			// Create variables to store information in
			char model[50], new_val[50], col[50];
			int id;

			// Extract results (strings)
			strcpy(col, gtk_combo_box_get_active_id(cmbUpdateDesktopCol));
			if (!strcmp(col, "Status_Code")) strcpy(new_val, gtk_entry_get_text(txtUpdateDesktopVal));
			else sprintf(new_val, "'%s'", gtk_entry_get_text(txtUpdateDesktopVal));
			strcpy(model, gtk_entry_get_text(txtUpdateDesktopModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numUpdateDesktopID);
			
			// Construct query and break
			sprintf(query, "UPDATE Desktops SET %s=%s WHERE ID=%d AND Model='%s';", col, new_val, id, model);
			break;

		}

		// Laptops
		case 1: {

			// Laptop Update Tools
			GtkEntry* txtUpdateLaptopModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateLaptopModel");
			GtkSpinButton* numUpdateLaptopID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numUpdateLaptopID");
			GtkComboBox* cmbUpdateLaptopCol = (GtkComboBox*) gtk_builder_get_object(args->builder, "cmbUpdateLaptopCol");
			GtkEntry* txtUpdateLaptopVal = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateLaptopVal");
			
			// Create variables to store information in
			char model[50], new_val[50], col[50];
			int id;

			// Extract results (strings)
			strcpy(col, gtk_combo_box_get_active_id(cmbUpdateLaptopCol));
			if (!strcmp(col, "Status_Code")) strcpy(new_val, gtk_entry_get_text(txtUpdateLaptopVal));
			else sprintf(new_val, "'%s'", gtk_entry_get_text(txtUpdateLaptopVal));
			strcpy(model, gtk_entry_get_text(txtUpdateLaptopModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numUpdateLaptopID);
			
			// Construct query and break
			sprintf(query, "UPDATE Laptops SET %s=%s WHERE ID=%d AND Model='%s';", col, new_val, id, model);
			break;

		}
		
		// Monitors
		case 2: {

			// Monitor Update Tools
			GtkEntry* txtUpdateMonitorModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateMonitorModel");
			GtkSpinButton* numUpdateMonitorID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numUpdateMonitorID");
			GtkComboBox* cmbUpdateMonitorCol = (GtkComboBox*) gtk_builder_get_object(args->builder, "cmbUpdateMonitorCol");
			GtkEntry* txtUpdateMonitorVal = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateMonitorVal");
			
			// Create variables to store information in
			char model[50], new_val[50], col[50];
			int id;

			// Extract results (strings)
			strcpy(col, gtk_combo_box_get_active_id(cmbUpdateMonitorCol));
			if (!strcmp(col, "Status_Code")) strcpy(new_val, gtk_entry_get_text(txtUpdateMonitorVal));
			else sprintf(new_val, "'%s'", gtk_entry_get_text(txtUpdateMonitorVal));
			strcpy(model, gtk_entry_get_text(txtUpdateMonitorModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numUpdateMonitorID);
			
			// Construct query and break
			sprintf(query, "UPDATE Monitors SET %s=%s WHERE ID=%d AND Model='%s';", col, new_val, id, model);
			break;

		}
			
		// Tablets
		case 3: {

			// Tablet Update Tools
			GtkEntry* txtUpdateTabletModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateTabletModel");
			GtkSpinButton* numUpdateTabletID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numUpdateTabletID");
			GtkComboBox* cmbUpdateTabletCol = (GtkComboBox*) gtk_builder_get_object(args->builder, "cmbUpdateTabletCol");
			GtkEntry* txtUpdateTabletVal = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateTabletVal");
			
			// Create variables to store information in
			char model[50], new_val[50], col[50];
			int id;

			// Extract results (strings)
			strcpy(col, gtk_combo_box_get_active_id(cmbUpdateTabletCol));
			if (!strcmp(col, "Status_Code")) strcpy(new_val, gtk_entry_get_text(txtUpdateTabletVal));
			else sprintf(new_val, "'%s'", gtk_entry_get_text(txtUpdateTabletVal));
			strcpy(model, gtk_entry_get_text(txtUpdateTabletModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numUpdateTabletID);
			
			// Construct query and break
			sprintf(query, "UPDATE Tablets SET %s=%s WHERE ID=%d AND Model='%s';", col, new_val, id, model);
			break;

		}
			
		// Phones
		case 4: {

			// Phone Update Tools
			GtkEntry* txtUpdatePhoneModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdatePhoneModel");
			GtkSpinButton* numUpdatePhoneID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numUpdatePhoneID");
			GtkComboBox* cmbUpdatePhoneCol = (GtkComboBox*) gtk_builder_get_object(args->builder, "cmbUpdatePhoneCol");
			GtkEntry* txtUpdatePhoneVal = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdatePhoneVal");
			
			// Create variables to store information in
			char model[50], new_val[50], col[50];
			int id;

			// Extract results (strings)
			strcpy(col, gtk_combo_box_get_active_id(cmbUpdatePhoneCol));
			if (!strcmp(col, "Status_Code")) strcpy(new_val, gtk_entry_get_text(txtUpdatePhoneVal));
			else sprintf(new_val, "'%s'", gtk_entry_get_text(txtUpdatePhoneVal));
			strcpy(model, gtk_entry_get_text(txtUpdatePhoneModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numUpdatePhoneID);
			
			// Construct query and break
			sprintf(query, "UPDATE Phones SET %s=%s WHERE ID=%d AND Model='%s';", col, new_val, id, model);
			break;

		}
			
		// Status
		case 5: {

			// Status Addition Tools
			GtkSpinButton* numUpdateStatusID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numUpdateStatusID");
			GtkComboBox* cmbUpdateStatusCol = (GtkComboBox*) gtk_builder_get_object(args->builder, "cmbUpdateStatusCol");
			GtkEntry* txtUpdateStatusVal = (GtkEntry*) gtk_builder_get_object(args->builder, "txtUpdateStatusVal");

			// Create variables to store information in
			char col[50], new_val[50];
			int id;

			// Extract results (strings)
			strcpy(col, gtk_combo_box_get_active_id(cmbUpdateStatusCol));
			if (!strcmp(col, "Priority")) strcpy(new_val, gtk_entry_get_text(txtUpdateStatusVal));
			else sprintf(new_val, "'%s'", gtk_entry_get_text(txtUpdateStatusVal));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numUpdateStatusID);
			
			// Construct query and break
			sprintf(query, "UPDATE Status SET %s=%s WHERE Status_Code=%d;", col, new_val, id);
			break;

		}

		// Error
		default: {

			// Display an error and return
			DisplayError(2, "Invalid page selected!");
			return;

		}	

	}

	// Set Status Indicator to sending command; also close the Insert Window
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "SENDING COMMAND");
	CloseWindow(NULL, args->winCRUD);

	// Execute command in sql.h
	if (runSQL(query) == 0) {
		DisplayError(0, "Command was executed successfully!");
		gtk_label_set_text(args->statusState, "COMMAND SUCCESS");
	} else {
		DisplayError(2, "An error was generated!");
		gtk_label_set_text(args->statusState, "COMMAND FAIL");
	}

	// Stop spinner 
	gtk_spinner_stop(args->statusBusy);

	// Show output
	GtkTextBuffer* buff2 = gtk_text_view_get_buffer(args->txtDisplay); 
	gtk_text_buffer_set_text(buff2, output, strlen(output));
	gtk_text_view_set_buffer(args->txtDisplay, buff2);
	strcpy(output, "\0");
	
}

// Function to update a database item
void DBUpdate(GtkWidget* sender, struct ParseCRUD_args *args) {

	// Create a new builder to show dialog box
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);

	// Get Insert Dialog
	GtkWidget* winUpdate = (GtkWidget*) gtk_builder_get_object(builder, "winUpdate");
	gtk_widget_show(winUpdate);
	args->winCRUD = (GtkWindow*) winUpdate;
	args->builder = builder;

	// Cancel Button
	GtkWidget* btnUpdateCancel = (GtkWidget*) gtk_builder_get_object(builder, "btnUpdateCancel");
	g_signal_connect(btnUpdateCancel, "clicked", G_CALLBACK(CloseWindow), winUpdate);

	// Go Button
	GtkWidget* btnUpdateGo = (GtkWidget*) gtk_builder_get_object(builder, "btnUpdateGo");
	g_signal_connect(btnUpdateGo, "clicked", G_CALLBACK(ParseUpdate), args);

	// Status showing waiting for input
	gtk_label_set_text(args->statusState, "WAITING FOR USER INPUT");

}

// Function to parse deletion of items from database
void ParseDelete(GtkWidget* sender, struct ParseCRUD_args *args) {

	// Array that contains the query
	char query[1024];

	// Set Status Indicator to parsing options
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "PARSING OPTIONS");

	// Get Notebook object to determine what page is used
	GtkNotebook* ntbDelete = (GtkNotebook*) gtk_builder_get_object(args->builder, "ntbDelete");

	// Find current page and create SQL Query
	switch (gtk_notebook_get_current_page(ntbDelete)) {

		// Desktops
		case 0: { 
			
			// Desktop Update Tools
			GtkEntry* txtDeleteDesktopModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtDeleteDesktopModel");
			GtkSpinButton* numDeleteDesktopID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numDeleteDesktopID");
			
			// Create variables to store information in
			char model[50];
			int id;

			// Extract results (strings)
			strcpy(model, gtk_entry_get_text(txtDeleteDesktopModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numDeleteDesktopID);
			
			// Construct query and break
			sprintf(query, "DELETE FROM Desktops WHERE ID=%d AND Model='%s';", id, model);
			break;

		}

		// Laptops
		case 1: {

			// Laptop Delete Tools
			GtkEntry* txtDeleteLaptopModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtDeleteLaptopModel");
			GtkSpinButton* numDeleteLaptopID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numDeleteLaptopID");
			
			// Create variables to store information in
			char model[50];
			int id;

			// Extract results (strings)
			strcpy(model, gtk_entry_get_text(txtDeleteLaptopModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numDeleteLaptopID);
			
			// Construct query and break
			sprintf(query, "DELETE FROM Laptops WHERE ID=%d AND Model='%s';", id, model);
			break;

		}
		
		// Monitors
		case 2: {

			// Monitor Delete Tools
			GtkEntry* txtDeleteMonitorModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtDeleteMonitorModel");
			GtkSpinButton* numDeleteMonitorID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numDeleteMonitorID");
			
			// Create variables to store information in
			char model[50];
			int id;

			// Extract results (strings)
			strcpy(model, gtk_entry_get_text(txtDeleteMonitorModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numDeleteMonitorID);
			
			// Construct query and break
			sprintf(query, "DELETE FROM Monitors WHERE ID=%d AND Model='%s';", id, model);
			break;

		}
			
		// Tablets
		case 3: {

			// Tablet Delete Tools
			GtkEntry* txtDeleteTabletModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtDeleteTabletModel");
			GtkSpinButton* numDeleteTabletID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numDeleteTabletID");
			
			// Create variables to store information in
			char model[50];
			int id;

			// Extract results (strings)
			strcpy(model, gtk_entry_get_text(txtDeleteTabletModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numDeleteTabletID);
			
			// Construct query and break
			sprintf(query, "DELETE FROM Tablets WHERE ID=%d AND Model='%s';", id, model);
			break;

		}
			
		// Phones
		case 4: {

			// Phone Delete Tools
			GtkEntry* txtDeletePhoneModel = (GtkEntry*) gtk_builder_get_object(args->builder, "txtDeletePhoneModel");
			GtkSpinButton* numDeletePhoneID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numDeletePhoneID");
			
			// Create variables to store information in
			char model[50];
			int id;

			// Extract results (strings)
			strcpy(model, gtk_entry_get_text(txtDeletePhoneModel));

			// Extract results (integers)
			id = gtk_spin_button_get_value_as_int(numDeletePhoneID);
			
			// Construct query and break
			sprintf(query, "DELETE FROM Phones WHERE ID=%d AND Model='%s';", id, model);
			break;

		}
			
		// Status
		case 5: {

			// Status Addition Tools
			GtkSpinButton* numDeleteStatusID = (GtkSpinButton*) gtk_builder_get_object(args->builder, "numDeleteStatusID");

			// Extract results (integers)
			int id = gtk_spin_button_get_value_as_int(numDeleteStatusID);
			
			// Construct query and break
			sprintf(query, "DELETE FROM Status WHERE Status_Code=%d;", id);
			break;

		}

		// Error
		default: {

			// Display an error and return
			DisplayError(2, "Invalid page selected!");
			return;

		}	

	}

	// Set Status Indicator to sending command; also close the Insert Window
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "SENDING COMMAND");
	CloseWindow(NULL, args->winCRUD);

	// Execute command in sql.h
	if (runSQL(query) == 0) {
		DisplayError(0, "Command was executed successfully!");
		gtk_label_set_text(args->statusState, "COMMAND SUCCESS");
	} else {
		DisplayError(2, "An error was generated!");
		gtk_label_set_text(args->statusState, "COMMAND FAIL");
	}

	// Stop spinner 
	gtk_spinner_stop(args->statusBusy);

	

	// Show output
	GtkTextBuffer* buff2 = gtk_text_view_get_buffer(args->txtDisplay); 
	gtk_text_buffer_set_text(buff2, output, strlen(output));
	gtk_text_view_set_buffer(args->txtDisplay, buff2);
	strcpy(output, "\0");
	
}

// Function to delete a database item
void DBDelete(GtkWidget* sender, struct ParseCRUD_args *args) {
	// Create a new builder to show dialog box
	GtkBuilder* builder = gtk_builder_new_from_file(GUI_FILE);

	// Get Delete Dialog
	GtkWidget* winDelete = (GtkWidget*) gtk_builder_get_object(builder, "winDelete");
	gtk_widget_show(winDelete);
	args->winCRUD = (GtkWindow*) winDelete;
	args->builder = builder;

	// Cancel Button
	GtkWidget* btnDeleteCancel = (GtkWidget*) gtk_builder_get_object(builder, "btnDeleteCancel");
	g_signal_connect(btnDeleteCancel, "clicked", G_CALLBACK(CloseWindow), winDelete);

	// Go Button
	GtkWidget* btnDeleteGo = (GtkWidget*) gtk_builder_get_object(builder, "btnDeleteGo");
	g_signal_connect(btnDeleteGo, "clicked", G_CALLBACK(ParseDelete), args);

	// Status showing waiting for input
	gtk_label_set_text(args->statusState, "WAITING FOR USER INPUT");
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
	char* query = (char*) gtk_text_buffer_get_text(buff, &c_start, &c_end, FALSE);
	
	// Close Window
	gtk_window_close(args->winCustom);
	
	// Show status as sending command
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "SENDING COMMAND");

	// Execute command in sql.h
	if (runSQL(query) == 0) {
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

// Function to dump a given table on the viewer
struct DBDump_args {
	GtkTextView* txtDisplay;
	GtkSpinner* statusBusy;
	GtkLabel* statusState;
};
void DBDump(GtkMenuItem* mnuSender, struct DBDump_args* args) {

	// Declare the string arrays needed to run the SQL command
	char clause[300];
	char query[1024];

	// Make status display that a table is being dumped
	gtk_spinner_start(args->statusBusy);
	gtk_label_set_text(args->statusState, "DUMPING TABLE");

	// Set the clause to contain the table being requested (given in the menu item text)
	sprintf(clause, "%s;", gtk_menu_item_get_label(mnuSender) + 5);

	// Create the query and add the clause to it
	strcpy(query, "SELECT * FROM ");
	strcat(query, clause);

	// Execute command; show status as failed if SQL fails
	if (runSQL(query) != 0) {
		gtk_spinner_stop(args->statusBusy);
		gtk_label_set_text(args->statusState, "FAIL");
		return;
	}

	// Show the status as done
	gtk_spinner_stop(args->statusBusy);
	gtk_label_set_text(args->statusState, "DONE");

	// Show and clean output buffer
	GtkTextBuffer* buff = gtk_text_view_get_buffer(args->txtDisplay); 
	gtk_text_buffer_set_text(buff, output, strlen(output));
	gtk_text_view_set_buffer(args->txtDisplay, buff);
	strcpy(output, "\0");
	
}
