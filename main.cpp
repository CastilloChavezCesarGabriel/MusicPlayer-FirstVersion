#include <QApplication>
#include "model/Model.h"
#include "view/View.h"
#include "controller/Controller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Model model;
    View view;
    Controller controller(&model, &view);
    (void)controller;
    view.show();
    return app.exec();
}
