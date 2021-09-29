// gtk_test.c

#include <gtk/gtk.h>

#include "gtk_test.h"

GtkWidget* gtk_window = NULL;

static void destroy_window(GtkWidget* widget, void* data);

void destroy_window(GtkWidget* widget, void* data) {
  gtk_main_quit();
}

void gtk_test_start(i32 argc, char** argv) {
  gtk_init(&argc, &argv);

  gtk_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  g_signal_connect(gtk_window, "destroy", G_CALLBACK(destroy_window), NULL);

  gtk_container_set_border_width(GTK_CONTAINER(gtk_window), 2);

  gtk_widget_show_all(gtk_window);

  gtk_main();
}

void gtk_test_exit() {

}
