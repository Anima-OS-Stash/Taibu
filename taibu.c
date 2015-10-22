/*
	Taibu - Desktop background setter that will be made heavier.
	
	Copyright (C) 2015 Minho Jo <whitestone8214@openmailbox.org>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// gcc -O3 -std=gnu99 $(pkg-config --cflags --libs gtkwidget2 libfile vte jansson) taibu.c

#include <gtkwidget2.h>
#include <libfile.h>
#include <vte/vte.h>
#include <gdk/gdkkeysyms.h>
#include <jansson.h>

int main (int argc, char *argv[]);
static void I_help_version (int *argc, char ***argv, const char *name, const char *brief, const char *command, const char *version);
static void I_opening (int *argc, char ***argv);
static void I_load_settings ();
static void I_knock_process (char *address, int *argc, char ***argv);
static void I_apply_settings (int nth, char new);

static void I_menu (char type);
static void I_add_page (char *command);
static void I_nth_page (int nth);
static void I_previous_page ();
static void I_next_page ();
static void I_remove_page (int nth);

static void I_set_title ();
static void I_clear_session ();
static void I_copy ();
static void I_paste ();
static void I_ask_remove_page ();
static void I_ask_quit ();
static void I_die_if_null (void *whatever);
static void I_default_settings ();
static void *I_quit ();

static GtkWidget *Q_box (char direction, int nContents, ...);
static GtkWidget *Q_window (char *title, char *icon, GtkWidget *content);
static GtkWidget *Q_button (GtkWidget *icon, char *tooltip, void *when_clicked, void *parcel);
static GtkWidget *Q_content (GtkWidget *from, int nSpots, ...);
static int Q_contents (GtkWidget *box);
static GtkWidget *Q_session (int nth);
static void Q_keybinding (GtkAccelGroup *keyring, guint key, void *when_pressed, void *parcel);
static json_t *Q_value (char *key);

static void *I_do_menu_A (void *whatever);
static void *I_do_menu_B (GtkButton *button, void *whatever);
static void *I_do_menu_C (void *whatever);
static void *I_do_menu_D (void *whatever);
static void *I_do_add_page_A (void *whatever);
static void *I_do_add_page_B (GtkButton *button, void *whatever);
static void *I_do_previous_page (void *whatever);
static void *I_do_next_page (void *whatever);
static void *I_do_remove_page_A (void *whatever);
static void *I_do_remove_page_B (VteTerminal *terminal, void *whatever);
static void *I_do_remove_page_C (GtkButton *button, void *whatever);

static void *I_do_pages (GtkButton *button, void *whatever);
static void *I_do_settings (GtkButton *button, void *whatever);
static void *I_do_about (GtkButton *button, void *whatever);
static void *I_do_nth_page (GtkButton *button, void *whatever);
static void *I_do_apply_settings (GtkButton *button, void *whatever);
static void *I_do_reload_settings (GtkButton *button, void *whatever);
static void *I_do_default_settings (GtkButton *button, void *whatever);
static void *I_do_show_shortcut_keys (GtkButton *button, void *whatever);
static void *I_do_show_copyright (GtkButton *button, void *whatever);
static void *I_do_set_title (VteTerminal *terminal, void *whatever);
static gboolean I_do_set_background_and_exit (void *whatever);
static void *I_do_clear_session (void *whatever);
static void *I_do_copy (void *whatever);
static void *I_do_paste (void *whatever);
static void *I_do_close_A (GtkButton *button, void *whatever);
static void *I_do_close_B (void *whatever);
static void *I_do_ask_remove_page_A (void *whatever);
static void *I_do_ask_remove_page_B (GtkButton *button, void *whatever);
static void *I_do_ask_quit_A (void *whatever);
static void *I_do_ask_quit_B (GtkWidget *widget, void *whatever);
static void *I_do_receive (GFileMonitor *watcher, GFile *file1, GFile *file2, GFileMonitorEvent event, void *whatever);

char *_argumentOpening, *_argumentImageFile;
json_t *_settings; int _session; GFile *_sessionMark; GFileMonitor *_sessionWatcher;
GtkWidget *_boxOverall, *_boxTitle; GdkGeometry _sizing;
GtkAccelGroup *_shortcuts;
int _nPages, _nthPage; GtkWidget **_labelTitle, **_boxBody;

char *_valueStartCommand; glong _valueScrollbackLines;
GdkColor _valueBackgroundColor; GdkPixbuf *_valueWallpaper; guint16 _valueBackgroundOpacity;
PangoFontDescription *_valueFont; GdkColor _valueTextColor;

int main (int argc, char *argv[]) {
	/* Prerequisities. */
	I_help_version (&argc, &argv, "Taibu", "Desktop background setter that will be made heavier.", "taibu [option OR image file]", "0.1");
	I_opening (&argc, &argv);
	_settings = NULL; _valueStartCommand = NULL; _valueWallpaper = NULL; _valueFont = NULL; I_load_settings ();
	I_knock_process ("/tmp/taibu.working", &argc, &argv);
	gtk_init (&argc, &argv); _nPages = 0; _nthPage = -1; _labelTitle = NULL; _boxBody = NULL;
	
	/* Layout. */
	/*GtkWidget *_window = Q_window ("Taibu", "terminal",
		Q_box (1, 1,
			Q_box (0, 4,
				Q_button (gtk_image_new_from_icon_name ("view-list-compact", GTK_ICON_SIZE_MENU), "Menu <b>(F10)</b>", I_do_menu_B, NULL),
				Q_box (0, 0),
				Q_button (gtk_image_new_from_icon_name ("add", GTK_ICON_SIZE_MENU), "New page <b>(Ctrl + Shift + T)</b>", I_do_add_page_B, NULL),
				Q_button (gtk_image_new_from_icon_name ("remove", GTK_ICON_SIZE_MENU), "Remove page <b>(Ctrl + Shift + W)</b>", I_do_ask_remove_page_B, NULL)
			)
		)
	);
	_boxOverall = Q_content (_window, 1, 0); _boxTitle = Q_content (_boxOverall, 2, 0, 1);*/
	
	/* Set details. */
	/*g_signal_connect (_window, "delete_event", (GCallback) I_do_ask_quit_B, NULL);
	_shortcuts = gtk_accel_group_new (); gtk_window_add_accel_group ((GtkWindow *) _window, _shortcuts);
	gtk_accel_group_connect (_shortcuts, GDK_KEY_F10, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_menu_A, NULL, NULL));
	gtk_accel_group_connect (_shortcuts, GDK_KEY_F11, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_menu_C, NULL, NULL));
	gtk_accel_group_connect (_shortcuts, GDK_KEY_F12, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_menu_D, NULL, NULL));
	Q_keybinding (_shortcuts, GDK_KEY_T, I_do_add_page_A, NULL);
	Q_keybinding (_shortcuts, GDK_KEY_Left, I_do_previous_page, NULL);
	Q_keybinding (_shortcuts, GDK_KEY_Right, I_do_next_page, NULL);
	Q_keybinding (_shortcuts, GDK_KEY_W, I_do_ask_remove_page_A, NULL);
	Q_keybinding (_shortcuts, GDK_KEY_Q, I_do_ask_quit_A, NULL);
	Q_keybinding (_shortcuts, GDK_KEY_R, I_do_clear_session, NULL);
	Q_keybinding (_shortcuts, GDK_KEY_C, I_do_copy, NULL);
	Q_keybinding (_shortcuts, GDK_KEY_V, I_do_paste, NULL);
	gtk_box_set_child_packing ((GtkBox *) Q_content (_boxOverall, 1, 0), _boxTitle, TRUE, TRUE, 0, GTK_PACK_START);*/
	
	/* New session. */
	/*I_add_page (NULL);
	gtk_widget_show_all (_window);
	int _width = vte_terminal_get_char_width ((VteTerminal *) Q_session (0)); int _height = vte_terminal_get_char_height ((VteTerminal *) Q_session (0));
	_sizing.min_width = _width; _sizing.base_width = _width; _sizing.width_inc = _width;
	_sizing.min_height = _height; _sizing.base_height = _height; _sizing.height_inc = _height;
	gtk_window_set_geometry_hints ((GtkWindow *) _window, Q_session (0), &_sizing, GDK_HINT_MIN_SIZE | GDK_HINT_BASE_SIZE | GDK_HINT_RESIZE_INC);
	gtk_widget_grab_focus (Q_session (0)); I_apply_settings (0, 'n');*/
	g_timeout_add (1, (GSourceFunc) I_do_set_background_and_exit, NULL);
	
	gtk_main ();
}
static void I_help_version (int *argc, char ***argv, const char *name, const char *brief, const char *command, const char *version) {
	if (*argc >= 2) {
		if (strcmp ((*argv)[1], "--help") == 0) {
			printf ("\n\
%s - %s\n\
\n\
Usage: %s\n\
\n\
Options:\n\
\t--help :: Show help and exit\n\
\t--version :: Show version and copyright\n\
\n", name, brief, command);
			exit (0);
		}
		else if (strcmp ((*argv)[1], "--version") == 0) {
			printf ("\n\
%s - %s\n\
\n\
Version %s\n\
\n\
Copyright (C) 2015 Minho Jo <whitestone8214@openmailbox.org>\n\
\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\
\n", name, brief, version);
			exit (0);
		}
	}
}
static void I_opening (int *argc, char ***argv) {
	_argumentOpening = NULL; _argumentImageFile = NULL;
	if ((*argc >= 2) && (strcmp ((*argv)[1], "--help") != 0) && (strcmp ((*argv)[1], "--version") != 0)) {
		char *_address = libfile_refine_address ((*argv)[1]); if (_address == NULL) {printf ("[Taibu] ERROR: Not enough memory or you did not specify a file.\n"); exit (1);}
		if (access (_address, R_OK) != 0) {printf ("[Taibu] ERROR: Must specify readable file.\n"); free (_address); exit (1);}
		char *_type = libfile_status (_address, 't'); if (_type == NULL) {printf ("[Taibu] ERROR: Not enough memory.\n"); free (_address); exit (1);}
		if (strcmp (_type, "Regular file") != 0) {printf ("[Taibu] ERROR: Must specify image file, not directory.\n"); free (_type); free (_address); exit (1);}
		free (_type); _argumentImageFile = _address;
	}
}
static void I_load_settings () {
	char *_address = libfile_refine_address ("~/data/taibu/settings.json"); if (_address == NULL) {printf ("[Taibu] ERROR: Not enough memory.\n"); exit (1);}
	if (_settings != NULL) {json_decref (_settings); _settings = NULL;} _settings = json_load_file (_address, 0, NULL);
	if (_settings == NULL) {I_default_settings (); _settings = json_load_file (_address, 0, NULL);}
	free (_address);
	
	if (_valueStartCommand != NULL) {free (_valueStartCommand); _valueStartCommand = NULL;}
	if (_valueWallpaper != NULL) {free (_valueWallpaper); _valueWallpaper = NULL;}
	if (_valueFont != NULL) {free (_valueFont); _valueFont = NULL;}
	
	json_t *_key;
	
	if (_argumentImageFile != NULL) _valueStartCommand = strdup (_argumentImageFile);
	else {
		_key = Q_value ("start-command"); _valueStartCommand = strdup (_key ? json_string_value (_key) : "/bin/env sh");
		if (_valueStartCommand == NULL) {printf ("[Taibu] ERROR: Taibu cannot work without proper start command.\n"); exit (1);}
		if (_key == NULL) printf ("[Taibu] WARNING: Value for key \"start-command\" not correctly set. Will use default value (\"/bin/env sh\").\n");
	}
	
	_key = Q_value ("scrollback-lines"); _valueScrollbackLines = _key ? (guint16) json_integer_value (_key) : 10000;
	if (_key == NULL) printf ("[Taibu] WARNING: Value for key \"scrollback-lines\" not correctly set. Will use default value (10000).\n");
	
	_key = Q_value ("background-color"); gboolean _answer = FALSE; if (_key) _answer = gdk_color_parse (json_string_value (_key), &_valueBackgroundColor);
	if ((_key == NULL) || (_answer == FALSE)) {
		_valueBackgroundColor.red = 65535; _valueBackgroundColor.green = 65535; _valueBackgroundColor.blue = 65535;
		printf ("[Taibu] WARNING: Value for key \"background-color\" not correctly set. Will use default value (\"#ffffffffffff\").\n");
	}
	
	if (_valueWallpaper != NULL) g_object_unref (_valueWallpaper);
	_key = Q_value ("wallpaper"); if (_key) _valueWallpaper = gdk_pixbuf_new_from_file (json_string_value (_key), NULL);
	if ((_key == NULL) || ((_valueWallpaper == NULL) && (strcmp (json_string_value (_key), "none") != 0))) printf ("[Taibu] WARNING: Value for key \"wallpaper\" not correctly set. Will use default value (\"none\").\n");
	
	_key = Q_value ("background-opacity"); _valueBackgroundOpacity = _key ? (guint16) json_integer_value (_key) : 65535;
	if (_key == NULL) printf ("[Taibu] WARNING: Value for key \"background-opacity\" not correctly set. Will use default value (65535).\n");
	
	if (_valueFont != NULL) pango_font_description_free (_valueFont);
	_key = Q_value ("font"); if (_key) _valueFont = _valueFont = pango_font_description_from_string (json_string_value (_key));
	if (_valueFont == NULL) {
		_valueFont = _valueFont = pango_font_description_from_string ("Monospace 10");
		if (_valueFont == NULL) {printf ("[Taibu] ERROR: Taibu cannot work without proper font.\n"); exit (1);}
		printf ("[Taibu] WARNING: Value for key \"font\" not correctly set. Will use default value (\"Monospace 10\").\n");
	}
	
	_key = Q_value ("text-color"); _answer = FALSE; if (_key) _answer = gdk_color_parse (json_string_value (_key), &_valueTextColor);
	if ((_key == NULL) || (_answer == FALSE)) {
		_valueTextColor.red = 0; _valueTextColor.green = 0; _valueTextColor.blue = 0;
		printf ("[Taibu] WARNING: Value for key \"text-color\" not correctly set. Will use default value (\"#000000000000\").\n");
	}
}
static void I_knock_process (char *address, int *argc, char ***argv) {
	_session = open (address, O_RDONLY); _sessionMark = NULL; _sessionWatcher = NULL;
	if (_session != -1) {
		truncate (address, 0); close (_session);
		if (*argc >= 2) {
			char *_command = strdup ("/bin/env"); if (_command == NULL) {printf ("[Taibu] ERROR: Not enough memory.\n"); exit (1);}
			int _nth; for (_nth = 1; _nth < *argc; _nth++) {
				char *_text = libtext_connect (3, _command, " ", (*argv)[_nth]); if (_text == NULL) {printf ("[Taibu] ERROR: Not enough memory.\n"); exit (1);}
				free (_command); _command = _text;
			}
			libfile_write (address, _command, 't', 0, 'n', NULL);
			_session = open (address, O_RDWR, 0600);
			write (_session, _command, sizeof (char) * (strlen (_command) + 1));
			write (_session, "\0", sizeof (char));
			close (_session);
			free (_command);
		}
		else printf ("[Taibu] It seems Taibu is already working. If isn't, try remove /tmp/taibu.working and run again.\n");
		exit (0);
	}
	else {
		_session = open (address, O_RDWR | O_CREAT, 0600);
		if (_session == -1) printf ("[Taibu] Can't write into /tmp! Will be unable to receive message from outside.\n");
		else {
			close (_session);
			_sessionMark = g_file_new_for_path (address);
			_sessionWatcher = g_file_monitor (_sessionMark, 0, NULL, NULL);
			g_signal_connect (_sessionWatcher, "changed", (GCallback) I_do_receive, NULL);
			if (_sessionWatcher == NULL) printf ("[Taibu] Can't watch /tmp/taibu.working! Will be unable to receive message from outside.\n");
		}
	}
}
static void I_apply_settings (int nth, char new) {
	if (new == 'y') {
		char *_textHere1 = libfile_here (); I_die_if_null (_textHere1);
		char **_command = libtext_split (_valueStartCommand, " "); I_die_if_null (_command);
		vte_terminal_fork_command_full ((VteTerminal *) Q_session (nth), VTE_PTY_DEFAULT, _textHere1, &_command[0], NULL, G_SPAWN_DEFAULT, NULL, NULL, NULL, NULL);
		int _nth = 0; while (_command[_nth] != NULL) {free (_command[_nth]); _nth++;} free (_command);
		free (_textHere1);
	}
	vte_terminal_set_scrollback_lines ((VteTerminal *) Q_session (nth), _valueScrollbackLines);
	vte_terminal_set_color_background ((VteTerminal *) Q_session (nth), &_valueBackgroundColor);
	if (_valueWallpaper != NULL) vte_terminal_set_background_image ((VteTerminal *) Q_session (nth), _valueWallpaper);
	vte_terminal_set_background_transparent ((VteTerminal *) Q_session (nth), _valueBackgroundOpacity != 65535);
	vte_terminal_set_opacity ((VteTerminal *) Q_session (nth), _valueBackgroundOpacity);
	vte_terminal_set_font ((VteTerminal *) Q_session (nth), _valueFont);
	vte_terminal_set_color_foreground ((VteTerminal *) Q_session (nth), &_valueTextColor);
}

static void I_menu (char type) {
	GtkWidget *_window = Q_window ("Taibu: menu", NULL,
		Q_box (1, 3,
			Q_box (0, 3,
				Q_button (gtk_image_new_from_icon_name ("view-list-compact", GTK_ICON_SIZE_MENU), "Pages", NULL, NULL),
				Q_button (gtk_image_new_from_icon_name ("preferences", GTK_ICON_SIZE_MENU), "Settings", NULL, NULL),
				Q_button (gtk_image_new_from_icon_name ("help", GTK_ICON_SIZE_MENU), "About this program", NULL, NULL)
			),
			Q_box (1, 0),
			gtk_button_new ()
		)
	);
	if (type == 1) I_do_settings (NULL, (void *) _window);
	else if (type == 2) I_do_about (NULL, (void *) _window);
	else I_do_pages (NULL, (void *) _window);
	
	gtk_window_set_position ((GtkWindow *) _window, GTK_WIN_POS_CENTER);
	gtk_window_set_modal ((GtkWindow *) _window, TRUE);
	GtkAccelGroup *_shortcuts1 = gtk_accel_group_new (); gtk_window_add_accel_group ((GtkWindow *) _window, _shortcuts1);
	gtk_accel_group_connect (_shortcuts1, GDK_KEY_Escape, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_close_B, (void *) _window, NULL));
	g_signal_connect (Q_content (_window, 3, 0, 0, 0), "clicked", (GCallback) I_do_pages, (void *) _window);
	g_signal_connect (Q_content (_window, 3, 0, 0, 1), "clicked", (GCallback) I_do_settings, (void *) _window);
	g_signal_connect (Q_content (_window, 3, 0, 0, 2), "clicked", (GCallback) I_do_about, (void *) _window);
	gtk_button_set_label ((GtkButton *) Q_content (_window, 2, 0, 2), "Close");
	g_signal_connect (Q_content (_window, 2, 0, 2), "clicked", (GCallback) I_do_close_A, (void *) _window);
	
	gtk_widget_show_all (_window);
}
static void I_add_page (char *command) {
	/* Withdraw current title and migrate to new title list. */
	GtkWidget *_labelTitle0 = Q_content (_boxTitle, 1, 0);
	if (_labelTitle0 != NULL) {g_object_ref (_labelTitle0); gtk_container_remove ((GtkContainer *) _boxTitle, _labelTitle0);}
	GtkWidget **_labelTitle1 = (GtkWidget **) malloc (sizeof (GtkWidget *) * (_nPages + 1)); I_die_if_null (_labelTitle1);
	int _nth; if (_nPages >= 1) {for (_nth = 0; _nth < _nPages; _nth++) _labelTitle1[_nth] = _labelTitle[_nth];}
	free (_labelTitle); _labelTitle = _labelTitle1;
	
	/* Withdraw current terminal box and migrate to new terminal box list. */
	GtkWidget *_boxBody0 = Q_content (_boxOverall, 1, 1);
	if (_boxBody0 != NULL) {g_object_ref (_boxBody0); gtk_container_remove ((GtkContainer *) _boxOverall, _boxBody0);}
	GtkWidget **_boxBody1 = (GtkWidget **) malloc (sizeof (GtkWidget *) * (_nPages + 1)); I_die_if_null (_boxBody1);
	if (_nPages >= 1) {for (_nth = 0; _nth < _nPages; _nth++) _boxBody1[_nth] = _boxBody[_nth];}
	free (_boxBody); _boxBody = _boxBody1;
	
	/* Add new title and terminal box. */
	int _nPagesOld = _nPages; _nPages++; _nthPage = _nPages - 1;
	_labelTitle1[_nthPage] = gtk_label_new ("(Untitled)"); gtk_box_pack_start ((GtkBox *) _boxTitle, _labelTitle1[_nthPage], FALSE, FALSE, 0);
	_boxBody1[_nthPage] = gtk_hbox_new (FALSE, 0); gtk_box_pack_start ((GtkBox *) _boxOverall, _boxBody1[_nthPage], TRUE, TRUE, 0);
	GtkWidget *_terminal = vte_terminal_new (); gtk_box_pack_start ((GtkBox *) _boxBody1[_nthPage], _terminal, TRUE, TRUE, 0);
	GtkWidget *_scroller = gtk_vscrollbar_new (NULL); gtk_box_pack_start ((GtkBox *) _boxBody1[_nthPage], _scroller, FALSE, FALSE, 0);
	I_apply_settings (_nthPage, 'y');
	
	/* Set details. */
	gtk_range_set_adjustment ((GtkRange *) _scroller, vte_terminal_get_adjustment ((VteTerminal *) _terminal));
	g_signal_connect (_terminal, "window_title_changed", (GCallback) I_do_set_title, (void *) _labelTitle);
	g_signal_connect (_terminal, "child_exited", (GCallback) I_do_remove_page_B, NULL);
	char *_title = (char *) vte_terminal_get_window_title ((VteTerminal *) _terminal); if (_title == NULL) _title = "(No title)";
	gtk_label_set_text ((GtkLabel *) _labelTitle1[_nthPage], _title);
	gtk_widget_grab_focus (_terminal);
	
	/* Go! */
	if (_nPagesOld >= 1) {gtk_widget_show_all (_boxOverall); gtk_widget_grab_focus (_boxBody[_nthPage]);}
}
static void I_nth_page (int nth) {
	/* Withdraw current title and migrate to new title list. */
	GtkWidget *_labelTitle0 = Q_content (_boxTitle, 1, 0);
	if (_labelTitle0 != NULL) {g_object_ref (_labelTitle0); gtk_container_remove ((GtkContainer *) _boxTitle, _labelTitle0);}
	GtkWidget **_labelTitle1 = (GtkWidget **) malloc (sizeof (GtkWidget *) * (_nPages + 1)); I_die_if_null (_labelTitle1);
	int _nth; if (_nPages >= 1) {for (_nth = 0; _nth < _nPages; _nth++) _labelTitle1[_nth] = _labelTitle[_nth];}
	free (_labelTitle); _labelTitle = _labelTitle1;
	
	/* Withdraw current terminal box and migrate to new terminal box list. */
	GtkWidget *_boxBody0 = Q_content (_boxOverall, 1, 1);
	if (_boxBody0 != NULL) {g_object_ref (_boxBody0); gtk_container_remove ((GtkContainer *) _boxOverall, _boxBody0);}
	GtkWidget **_boxBody1 = (GtkWidget **) malloc (sizeof (GtkWidget *) * (_nPages + 1)); I_die_if_null (_boxBody1);
	if (_nPages >= 1) {for (_nth = 0; _nth < _nPages; _nth++) _boxBody1[_nth] = _boxBody[_nth];}
	free (_boxBody); _boxBody = _boxBody1;
	
	/* Show wanted page. */
	_nthPage = nth;
	if (_nPages >= 1) {
		gtk_box_pack_start ((GtkBox *) _boxTitle, _labelTitle[_nthPage], FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox *) _boxOverall, _boxBody[_nthPage], TRUE, TRUE, 0);
		gtk_widget_show_all (_boxBody[_nthPage]); gtk_widget_grab_focus (_boxBody[_nthPage]);
	}
}
static void I_previous_page () {if (_nthPage >= 1) I_nth_page (_nthPage - 1); else I_nth_page (_nPages - 1);}
static void I_next_page () {if (_nthPage < _nPages - 1) I_nth_page (_nthPage + 1); else I_nth_page (0);}
static void I_remove_page (int nth) {
	if ((nth < 0) || (nth >= _nPages)) return;
	
	/* Withdraw title and migrate to new title list. */
	gtk_widget_destroy (_labelTitle[nth]); _labelTitle[nth] = NULL;
	if (_nPages >= 2) {
		GtkWidget **_labelTitle1 = (GtkWidget **) malloc (sizeof (GtkWidget *) * (_nPages - 1)); I_die_if_null (_labelTitle1);
		int _nth; int _nth1 = 0; 
		for (_nth = 0; _nth < _nPages; _nth++) {
			if (_labelTitle[_nth] == NULL) continue;
			_labelTitle1[_nth1] = _labelTitle[_nth]; _nth1++;
		}
		free (_labelTitle); _labelTitle = _labelTitle1;
	}
	
	/* Withdraw terminal box and migrate to new terminal box. */
	gtk_widget_destroy (_boxBody[nth]); _boxBody[nth] = NULL;
	if (_nPages >= 2) {
		GtkWidget **_boxBody1 = (GtkWidget **) malloc (sizeof (GtkWidget *) * (_nPages - 1)); I_die_if_null (_boxBody1);
		int _nth; int _nth1 = 0; 
		for (_nth = 0; _nth < _nPages; _nth++) {
			if (_boxBody[_nth] == NULL) continue;
			_boxBody1[_nth1] = _boxBody[_nth]; _nth1++;
		}
		free (_boxBody); _boxBody = _boxBody1;
	}
	
	/* Show another page or quit. */
	_nPages--; _nthPage = nth == 0 ? 0 : nth - 1;
	if (_nPages >= 1) {
		gtk_box_pack_start ((GtkBox *) _boxTitle, _labelTitle[_nthPage], FALSE, FALSE, 0);
		gtk_box_pack_start ((GtkBox *) _boxOverall, _boxBody[_nthPage], TRUE, TRUE, 0);
		gtk_widget_show_all (_boxBody[_nthPage]); gtk_widget_grab_focus (_boxBody[_nthPage]);
	}
	else {free (_labelTitle); free (_boxBody); I_quit ();}
}

static void I_set_title () {
	char *_title = (char *) vte_terminal_get_window_title ((VteTerminal *) Q_session (_nthPage)); if (_title == NULL) _title = "(No title)";
	gtk_label_set_text ((GtkLabel *) _labelTitle[_nthPage], _title);
}
static void I_clear_session () {vte_terminal_reset ((VteTerminal *) Q_session (_nthPage), TRUE, TRUE); TRUE;}
static void I_copy () {
	gtk_widget_grab_focus (gtk_widget_get_toplevel (_boxOverall));
	vte_terminal_copy_clipboard ((VteTerminal *) Q_session (_nthPage));
	gtk_widget_grab_focus (Q_session (_nthPage));
}
static void I_paste () {vte_terminal_paste_clipboard ((VteTerminal *) Q_session (_nthPage)); TRUE;}
static void I_ask_remove_page () {
	GtkWidget *_window = Q_window ("Close this session", NULL,
		Q_box (0, 3,
			gtk_label_new ("Are you sure? (Y/N)"),
			gtk_button_new (),
			gtk_button_new ()
		)
	);
	GtkWidget *_buttonY = Q_content (_window, 2, 0, 1); GtkWidget *_buttonN = Q_content (_window, 2, 0, 2);
	
	gtk_window_set_position ((GtkWindow *) _window, GTK_WIN_POS_CENTER);
	gtk_window_set_modal ((GtkWindow *) _window, TRUE);
	GtkAccelGroup *_shortcuts1 = gtk_accel_group_new (); gtk_window_add_accel_group ((GtkWindow *) _window, _shortcuts1);
	gtk_widget_set_tooltip_markup (_buttonY, "Yes <b>(Y)</b>");
	g_signal_connect (_buttonY, "clicked", (GCallback) I_do_remove_page_C, (void *) _window);
	gtk_accel_group_connect (_shortcuts1, GDK_KEY_Y, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_remove_page_A, (void *) _window, NULL));
	gtk_widget_set_tooltip_markup (_buttonN, "No <b>(N)</b>");
	g_signal_connect (_buttonN, "clicked", (GCallback) I_do_close_A, (void *) _window);
	gtk_accel_group_connect (_shortcuts1, GDK_KEY_N, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_close_B, (void *) _window, NULL));
	gtk_accel_group_connect (_shortcuts1, GDK_KEY_Escape, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_close_B, (void *) _window, NULL));
	gtk_button_set_image ((GtkButton *) _buttonY, gtk_image_new_from_icon_name ("ok", GTK_ICON_SIZE_MENU));
	gtk_button_set_image ((GtkButton *) _buttonN, gtk_image_new_from_icon_name ("cancel", GTK_ICON_SIZE_MENU));
	
	gtk_widget_show_all (_window);
}
static void I_ask_quit () {
	GtkWidget *_window = Q_window ("Quit", NULL,
		Q_box (0, 3,
			gtk_label_new ("Are you sure? (Y/N)"),
			gtk_button_new (),
			gtk_button_new ()
		)
	);
	GtkWidget *_buttonY = Q_content (_window, 2, 0, 1); GtkWidget *_buttonN = Q_content (_window, 2, 0, 2);
	
	gtk_window_set_position ((GtkWindow *) _window, GTK_WIN_POS_CENTER);
	gtk_window_set_modal ((GtkWindow *) _window, TRUE);
	GtkAccelGroup *_shortcuts1 = gtk_accel_group_new (); gtk_window_add_accel_group ((GtkWindow *) _window, _shortcuts1);
	gtk_widget_set_tooltip_markup (_buttonY, "Yes <b>(Y)</b>");
	g_signal_connect (_buttonY, "clicked", (GCallback) I_quit, NULL);
	gtk_accel_group_connect (_shortcuts1, GDK_KEY_Y, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_quit, NULL, NULL));
	gtk_widget_set_tooltip_markup (_buttonN, "No <b>(N)</b>");
	g_signal_connect (_buttonN, "clicked", (GCallback) I_do_close_A, (void *) _window);
	gtk_accel_group_connect (_shortcuts1, GDK_KEY_N, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_close_B, (void *) _window, NULL));
	gtk_accel_group_connect (_shortcuts1, GDK_KEY_Escape, 0, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) I_do_close_B, (void *) _window, NULL));
	gtk_button_set_image ((GtkButton *) _buttonY, gtk_image_new_from_icon_name ("ok", GTK_ICON_SIZE_MENU));
	gtk_button_set_image ((GtkButton *) _buttonN, gtk_image_new_from_icon_name ("cancel", GTK_ICON_SIZE_MENU));
	
	gtk_widget_show_all (_window);
}
static void I_die_if_null (void *whatever) {if (whatever == NULL) {printf ("[Taibu] ERROR: Not enough memory.\n"); exit (1);}}
static void I_default_settings () {
	/* Delete and re-create settings file. */
	char *_address = libfile_refine_address ("~/data/taibu/settings.json"); if (_address == NULL) {printf ("[Taibu] ERROR: Not enough memory.\n"); exit (1);}
	if (libfile_create (_address, 'f', 'y') != 's') {printf ("[Taibu] ERROR: Taibu cannot work without settings file.\n"); exit (1);}
	if (libfile_erase (_address, NULL) != 's') {printf ("[Taibu] ERROR: Taibu cannot work without settings file.\n"); exit (1);}
	libfile_create (_address, 'f', 'y');
	
	/* Re-write default settings. */
	json_object_clear (_settings); json_decref (_settings); _settings = json_object ();
	json_object_set (_settings, "start-command", json_string ("/bin/env sh"));
	json_object_set (_settings, "scrollback-lines", json_integer (10000));
	json_object_set (_settings, "background-color", json_string ("#ffffffffffff"));
	json_object_set (_settings, "wallpaper", json_string ("none"));
	json_object_set (_settings, "background-opacity", json_integer (65535));
	json_object_set (_settings, "font", json_string ("Monospace 10"));
	json_object_set (_settings, "text-color", json_string ("#000000000000"));
	json_dump_file (_settings, _address, JSON_INDENT(2));
	free (_address);
}
static void *I_quit () {
	if (_argumentImageFile != NULL) free (_argumentImageFile);
	unlink ("/tmp/taibu.working");
	gtk_main_quit ();
}

static GtkWidget *Q_box (char direction, int nContents, ...) {
	GtkWidget *_box; if (direction == 1) _box = gtk_vbox_new (FALSE, 0); else _box = gtk_hbox_new (FALSE, 0);
	va_list _list; va_start (_list, nContents);
	int _nth; for (_nth = 0; _nth < nContents; _nth++) {
		GtkWidget *_content = va_arg (_list, GtkWidget *);
		if (_content != NULL) gtk_box_pack_start ((GtkBox *) _box, _content, FALSE, FALSE, 0);
	}
	va_end (_list);
	return _box;
}
static GtkWidget *Q_window (char *title, char *icon, GtkWidget *content) {
	GtkWidget *_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	if (title != NULL) gtk_window_set_title ((GtkWindow *) _window, title);
	if (icon != NULL) gtk_window_set_icon_name ((GtkWindow *) _window, icon);
	if (content != NULL) gtk_container_add ((GtkContainer *) _window, content);
	return _window;
}
static GtkWidget *Q_button (GtkWidget *icon, char *tooltip, void *when_clicked, void *parcel) {
	GtkWidget *_button = gtk_button_new ();
	if (icon != NULL) gtk_button_set_image ((GtkButton *) _button, icon);
	if (tooltip != NULL) gtk_widget_set_tooltip_markup (_button, tooltip);
	if (when_clicked != NULL) g_signal_connect (_button, "clicked", (GCallback) when_clicked, parcel);
	return _button;
}
static GtkWidget *Q_content (GtkWidget *from, int nSpots, ...) {
	va_list _list; va_start (_list, nSpots); GtkWidget *_this = from;
	int _nth; for (_nth = 0; _nth < nSpots; _nth++) {
		int _nth1 = va_arg (_list, int);
		if (_nth1 < 0) _this = gtk_widget_get_parent (_this);
		else {
			GList *_list1 = gtk_container_get_children ((GtkContainer *) _this); if (_list1 == NULL) return NULL;
			if (_nth1 >= g_list_length (_list1)) {g_list_free (_list1); return NULL;}
			_this = g_list_nth_data (_list1, _nth1); g_list_free (_list1);
		}
	}
	va_end (_list); return _this;
}
static int Q_contents (GtkWidget *box) {
	GList *_list = gtk_container_get_children ((GtkContainer *) box);
	int _nContents = g_list_length (_list); g_list_free (_list);
	return _nContents;
}
static GtkWidget *Q_session (int nth) {return Q_content (_boxBody[nth], 1, 0);}
static void Q_keybinding (GtkAccelGroup *keyring, guint key, void *when_pressed, void *parcel) {
	if (when_pressed == NULL) return;
	gtk_accel_group_connect (keyring, key, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE, g_cclosure_new_swap ((GCallback) when_pressed, parcel, NULL));
}
static json_t *Q_value (char *key) {return json_object_get (_settings, key);}

static void *I_do_menu_A (void *whatever) {I_menu (0);}
static void *I_do_menu_B (GtkButton *button, void *whatever) {I_menu (0);}
static void *I_do_menu_C (void *whatever) {I_menu (1);}
static void *I_do_menu_D (void *whatever) {I_menu (2);}
static void *I_do_add_page_A (void *whatever) {I_add_page (NULL);}
static void *I_do_add_page_B (GtkButton *button, void *whatever) {I_add_page (NULL);}
static void *I_do_previous_page (void *whatever) {I_previous_page ();}
static void *I_do_next_page (void *whatever) {I_next_page ();}
static void *I_do_remove_page_A (void *whatever) {if (whatever != NULL) gtk_widget_destroy ((GtkWidget *) whatever); I_remove_page (_nthPage);}
static void *I_do_remove_page_B (VteTerminal *terminal, void *whatever) {I_remove_page (_nthPage);}
static void *I_do_remove_page_C (GtkButton *button, void *whatever) {if (whatever != NULL) gtk_widget_destroy ((GtkWidget *) whatever); I_remove_page (_nthPage);}

static void *I_do_pages (GtkButton *button, void *whatever) {
	if (Q_contents (Q_content ((GtkWidget *) whatever, 2, 0, 1)) == 1) gtk_widget_destroy (Q_content ((GtkWidget *) whatever, 3, 0, 1, 0));
	GtkWidget *_box = gtk_vbox_new (FALSE, 0);
	int _nth; for (_nth = 0; _nth < _nPages; _nth++) {
		GtkWidget *_buttonPage = gtk_button_new ();
		char *_labelPage = libtext_number (_nth); I_die_if_null (_labelPage);
		gtk_widget_set_name (_buttonPage, _labelPage); free (_labelPage);
		gtk_button_set_label ((GtkButton *) _buttonPage, gtk_label_get_text ((GtkLabel *) _labelTitle[_nth]));
		g_signal_connect (_buttonPage, "clicked", (GCallback) I_do_nth_page, whatever);
		gtk_box_pack_start ((GtkBox *) _box, _buttonPage, TRUE, TRUE, 0);
	}
	gtk_box_pack_start ((GtkBox *) Q_content ((GtkWidget *) whatever, 2, 0, 1), _box, TRUE, TRUE, 0);
	gtk_widget_show_all (_box);
}
static void *I_do_settings (GtkButton *button, void *whatever) {
	if (Q_contents (Q_content ((GtkWidget *) whatever, 2, 0, 1)) == 1) gtk_widget_destroy (Q_content ((GtkWidget *) whatever, 3, 0, 1, 0));
	GtkWidget *_box = Q_box (1, 8,
		Q_box (0, 2, gtk_label_new ("Start command: "), gtk_entry_new ()),
		Q_box (0, 2, gtk_label_new ("Scrollback lines: "), gtk_entry_new ()),
		Q_box (0, 2, gtk_label_new ("Background color: "), gtk_entry_new ()),
		Q_box (0, 2, gtk_label_new ("Wallpaper: "), gtk_entry_new ()),
		Q_box (0, 2, gtk_label_new ("Background opacity: "), gtk_entry_new ()),
		Q_box (0, 2, gtk_label_new ("Font: "), gtk_entry_new ()),
		Q_box (0, 2, gtk_label_new ("Text color: "), gtk_entry_new ()),
		Q_box (0, 3, gtk_button_new_with_label ("Apply"), gtk_button_new_with_label ("Reload"), gtk_button_new_with_label ("Default"))
	);
	gtk_box_pack_start ((GtkBox *) Q_content ((GtkWidget *) whatever, 2, 0, 1), _box, TRUE, TRUE, 0);
	gtk_widget_show_all (_box);
	
	json_t *_key = Q_value ("start-command"); gtk_entry_set_text ((GtkEntry *) Q_content (_box, 2, 0, 1), json_string_value (_key));
	_key = Q_value ("scrollback-lines"); char *_number = libtext_number ((guint16) json_integer_value (_key)); gtk_entry_set_text ((GtkEntry *) Q_content (_box, 2, 1, 1), _number); free (_number);
	_key = Q_value ("background-color"); gtk_entry_set_text ((GtkEntry *) Q_content (_box, 2, 2, 1), json_string_value (_key));
	_key = Q_value ("wallpaper"); gtk_entry_set_text ((GtkEntry *) Q_content (_box, 2, 3, 1), json_string_value (_key));
	_key = Q_value ("background-opacity"); _number = libtext_number ((guint16) json_integer_value (_key)); gtk_entry_set_text ((GtkEntry *) Q_content (_box, 2, 4, 1), _number); free (_number);
	_key = Q_value ("font"); gtk_entry_set_text ((GtkEntry *) Q_content (_box, 2, 5, 1), json_string_value (_key));
	_key = Q_value ("text-color"); gtk_entry_set_text ((GtkEntry *) Q_content (_box, 2, 6, 1), json_string_value (_key));
	
	g_signal_connect (Q_content (_box, 2, 7, 0), "clicked", (GCallback) I_do_apply_settings, whatever);
	g_signal_connect (Q_content (_box, 2, 7, 1), "clicked", (GCallback) I_do_reload_settings, whatever);
	g_signal_connect (Q_content (_box, 2, 7, 2), "clicked", (GCallback) I_do_default_settings, whatever);
}
static void *I_do_about (GtkButton *button, void *whatever) {
	if (Q_contents (Q_content ((GtkWidget *) whatever, 2, 0, 1)) == 1) gtk_widget_destroy (Q_content ((GtkWidget *) whatever, 3, 0, 1, 0));
	GtkWidget *_box = Q_box (1, 3,
		Q_box (0, 2,
			gtk_image_new_from_icon_name ("terminal", GTK_ICON_SIZE_DIALOG),
			Q_box (1, 2,
				Q_box (0, 1, gtk_label_new ("<b>Taibu</b>")),
				gtk_label_new ("Middleweight terminal emulator")
			)
		),
		Q_box (0, 2, gtk_button_new_with_label ("Shortcut keys"), gtk_button_new_with_label ("Copyright")),
		gtk_text_view_new ()
	);
	gtk_box_pack_start ((GtkBox *) Q_content ((GtkWidget *) whatever, 2, 0, 1), _box, TRUE, TRUE, 0);
	gtk_widget_show_all (_box);
	
	gtk_label_set_use_markup ((GtkLabel *) Q_content (_box, 4, 0, 1, 0, 0), TRUE);
	g_signal_connect (Q_content (_box, 2, 1, 0), "clicked", (GCallback) I_do_show_shortcut_keys, whatever);
	g_signal_connect (Q_content (_box, 2, 1, 1), "clicked", (GCallback) I_do_show_copyright, whatever);
	gtk_text_view_set_editable ((GtkTextView *) Q_content (_box, 1, 2), FALSE);
	
	I_do_show_shortcut_keys (NULL, whatever);
}
static void *I_do_nth_page (GtkButton *button, void *whatever) {
	int _nth = atoi (gtk_widget_get_name ((GtkWidget *) button));
	gtk_widget_destroy ((GtkWidget *) whatever);
	I_nth_page (_nth);
}
static void *I_do_apply_settings (GtkButton *button, void *whatever) {
	json_object_clear (_settings); json_decref (_settings); _settings = json_object ();
	json_object_set (_settings, "start-command", json_string (gtk_entry_get_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 0, 1))));
	json_object_set (_settings, "scrollback-lines", json_integer (atol (gtk_entry_get_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 1, 1)))));
	json_object_set (_settings, "background-color", json_string (gtk_entry_get_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 2, 1))));
	json_object_set (_settings, "wallpaper", json_string (gtk_entry_get_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 3, 1))));
	json_object_set (_settings, "background-opacity", json_integer (atol (gtk_entry_get_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 4, 1)))));
	json_object_set (_settings, "font", json_string (gtk_entry_get_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 5, 1))));
	json_object_set (_settings, "text-color", json_string (gtk_entry_get_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 6, 1))));
	
	char *_address = libfile_refine_address ("~/data/taibu/settings.json"); if (_address == NULL) {printf ("[Taibu] ERROR: Not enough memory.\n"); exit (1);}
	json_dump_file (_settings, _address, JSON_INDENT(2));
	free (_address);
	
	I_load_settings (); int _nth; for (_nth = 0; _nth < _nPages; _nth++) I_apply_settings (_nth, 'n');
	gtk_widget_destroy ((GtkWidget *) whatever);
}
static void *I_do_reload_settings (GtkButton *button, void *whatever) {
	json_t *_key = Q_value ("start-command"); gtk_entry_set_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 0, 1), json_string_value (_key));
	_key = Q_value ("scrollback-lines"); char *_number = libtext_number ((guint16) json_integer_value (_key)); gtk_entry_set_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 1, 1), _number); free (_number);
	_key = Q_value ("background-color"); gtk_entry_set_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 2, 1), json_string_value (_key));
	_key = Q_value ("wallpaper"); gtk_entry_set_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 3, 1), json_string_value (_key));
	_key = Q_value ("background-opacity"); _number = libtext_number ((guint16) json_integer_value (_key)); gtk_entry_set_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 4, 1), _number); free (_number);
	_key = Q_value ("font"); gtk_entry_set_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 5, 1), json_string_value (_key));
	_key = Q_value ("text-color"); gtk_entry_set_text ((GtkEntry *) Q_content ((GtkWidget *) whatever, 5, 0, 1, 0, 6, 1), json_string_value (_key));
}
static void *I_do_default_settings (GtkButton *button, void *whatever) {
	I_default_settings ();
	I_do_reload_settings (NULL, whatever);
	gtk_widget_destroy ((GtkWidget *) whatever);
}
static void *I_do_show_shortcut_keys (GtkButton *button, void *whatever) {
	GtkTextBuffer *_content = gtk_text_buffer_new (NULL);
	gtk_text_buffer_set_text (_content, "Ctrl + Shift + T = New session\n\
Ctrl + Shift + Left = Previous session\n\
Ctrl + Shift + Right = Next session\n\
Ctrl + Shift + W = Close this session\n\
Ctrl + Shift + Q = Quit\n\
Ctrl + Shift + R = Clear scrollback\n\
Ctrl + Shift + C = Copy to clipboard\n\
Ctrl + Shift + V = Paste clipboard\n\
F10 = Menu\n\
F11 = Settings\n\
F12 = About this program", -1);
	
	gtk_text_view_set_buffer ((GtkTextView *) Q_content ((GtkWidget *) whatever, 4, 0, 1, 0, 2), _content);
}
static void *I_do_show_copyright (GtkButton *button, void *whatever) {
	GtkTextBuffer *_content = gtk_text_buffer_new (NULL);
	gtk_text_buffer_set_text (_content, "Copyright (C) 2015 Minho Jo <whitestone8214@openmailbox.org>\n\
\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program.  If not, see <http://www.gnu.org/licenses/>.", -1);
	
	gtk_text_view_set_buffer ((GtkTextView *) Q_content ((GtkWidget *) whatever, 4, 0, 1, 0, 2), _content);
}
static void *I_do_set_title (VteTerminal *terminal, void *whatever) {I_set_title ();}
static gboolean I_do_set_background_and_exit (void *whatever) {
	GdkPixbuf *_picture = gdk_pixbuf_new_from_file (_argumentImageFile, NULL);
	if (_picture == NULL) {printf ("[Taibu] ERROR: It seems you specified file that Taibu does not support for now.\n"); free (_argumentImageFile); exit (1);}
	float _widthPicture = (float) gdk_pixbuf_get_width (_picture); float _heightPicture = (float) gdk_pixbuf_get_height (_picture);
	float _aspectPicture = (_widthPicture > _heightPicture) ? (_widthPicture / _heightPicture) : (_heightPicture / _widthPicture);
	
	GdkWindow *_background = gdk_screen_get_root_window (gdk_screen_get_default ());
	float _widthBackground = (float) gdk_window_get_width (_background); float _heightBackground = (float) gdk_window_get_height (_background);
	float _aspectBackground = (_widthBackground > _heightBackground) ? (_widthBackground / _heightBackground) : (_heightBackground / _widthBackground);
	
	float _width, _height;
	if (_aspectBackground > _aspectPicture) {
		if (_widthBackground > _heightBackground) {_width = _widthBackground; _height = _heightPicture * (_widthBackground / _widthPicture);}
		else {_width = _widthPicture * (_heightBackground / _heightPicture); _height = _heightBackground;}
	}
	else {
		if (_widthPicture > _heightPicture) {_width = _widthPicture * (_heightBackground / _heightPicture); _height = _heightBackground;}
		else {_width = _widthBackground; _height = _heightPicture * (_widthBackground / _widthPicture);}
	}
	
	GdkPixmap *_picture1 = gdk_pixmap_new ((GdkDrawable *) _background, (int) _widthBackground, (int) _heightBackground, -1);
	gdk_draw_pixbuf (_picture1, NULL, gdk_pixbuf_new_from_file_at_size (_argumentImageFile, (int) _width, (int) _height, NULL), 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_MAX, 0, 0);
	gdk_window_set_back_pixmap (_background, _picture1, FALSE);
	gdk_window_clear (_background);
	
	I_quit (); return TRUE;
}
static void *I_do_clear_session (void *whatever) {I_clear_session ();}
static void *I_do_copy (void *whatever) {I_copy ();}
static void *I_do_paste (void *whatever) {I_paste ();}
static void *I_do_close_A (GtkButton *button, void *whatever) {if (whatever != NULL) gtk_widget_destroy ((GtkWidget *) whatever);}
static void *I_do_close_B (void *whatever) {if (whatever != NULL) gtk_widget_destroy ((GtkWidget *) whatever);}
static void *I_do_ask_remove_page_A (void *whatever) {I_ask_remove_page ();}
static void *I_do_ask_remove_page_B (GtkButton *button, void *whatever) {I_ask_remove_page ();}
static void *I_do_ask_quit_A (void *whatever) {I_ask_quit ();}
static void *I_do_ask_quit_B (GtkWidget *widget, void *whatever) {I_ask_quit ();}
static void *I_do_receive (GFileMonitor *watcher, GFile *file1, GFile *file2, GFileMonitorEvent event, void *whatever) {
	if (event == G_FILE_MONITOR_EVENT_CHANGED) {
		char *_content = libfile_read ("/tmp/taibu.working", 0, -1);
		if (_content != NULL) {I_add_page (_content); free (_content);}
	}
}
