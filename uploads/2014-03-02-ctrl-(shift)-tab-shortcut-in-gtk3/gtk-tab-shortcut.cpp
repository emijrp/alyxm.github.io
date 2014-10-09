#include <gtk/gtk.h>

bool controlPressed;

// user_data = if this is a callback for a keydown (else a keyup)
bool control_key_checker(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	if (event->keyval == GDK_KEY_Control_L)
		controlPressed = (bool)user_data;
	
	return false;
}

bool notebook_tab_shortcut(GtkWidget *widget, GtkDirectionType direction, gpointer user_data)
{
	if (controlPressed)
	{
		int last    = gtk_notebook_get_n_pages(GTK_NOTEBOOK(user_data)) - 1;
		int current = gtk_notebook_get_current_page(GTK_NOTEBOOK(user_data));
		int next;
		
		if (direction)
			next = (current == 0 ? last : current - 1);
		else
			next = (current == last ? 0 : current + 1);
		
		gtk_notebook_set_current_page(GTK_NOTEBOOK(user_data), next);
	}
	
	return true;
}

GtkWidget *new_window()
{
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Grid");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	
	return window;
}

GtkWidget *new_grid()
{
	GtkWidget *grid = gtk_grid_new();
	
	return grid;
}

GtkWidget *new_notebook()
{
	GtkWidget *notebook = gtk_notebook_new ();
	
	for (int i = 0; i < 5; ++i)
		gtk_notebook_append_page(GTK_NOTEBOOK (notebook), gtk_button_new_with_label("Button"), nullptr);
	
	gtk_widget_set_hexpand(notebook, true);
	gtk_widget_set_vexpand(notebook, true);
	
	return notebook;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	
	GtkWidget *window = new_window();
	GtkWidget *grid   = new_grid();
	GtkWidget *notebook = new_notebook();
	
	gtk_container_add(GTK_CONTAINER(window), grid);
	
	g_signal_connect(window, "focus", G_CALLBACK(notebook_tab_shortcut), (gpointer)notebook);
	g_signal_connect(window, "key-press-event",   G_CALLBACK(control_key_checker), (gpointer)true);
	g_signal_connect(window, "key-release-event", G_CALLBACK(control_key_checker), (gpointer)false);
	
	gtk_grid_attach(GTK_GRID(grid), notebook,  0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), gtk_button_new_with_label("Button"), 0, 1, 1, 1);
	
	gtk_widget_show_all(window);
	gtk_main();
	
	return 0;
}
