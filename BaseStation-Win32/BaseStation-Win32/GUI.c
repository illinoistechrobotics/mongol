#include "GUI.h"

GtkWidget * window;

void initGUI (int argc, char * argv[]){

    gtk_set_locale();
    gtk_init(&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show(window);
    gtk_main();
}