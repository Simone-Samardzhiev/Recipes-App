#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <QLineEdit>
#include <utility>
#include <vector>
#include "Functions.h"

// Assume you have the necessary definitions for Data and Dish classes

class NewRecipe : public QWidget {
Q_OBJECT
private:
    Data data;
    QGridLayout *layout = new QGridLayout;
    QLabel *label1 = new QLabel("Enter the name:");
    QLineEdit *nameLineEdit = new QLineEdit;
    QLabel *label2 = new QLabel("Enter the ingredients");
    QTextEdit *textForIngredients = new QTextEdit;
    QLabel *label3 = new QLabel("Enter how it is made");
    QTextEdit *textForMaking = new QTextEdit;
    QComboBox *options = new QComboBox;
    QPushButton *button = new QPushButton("create");

signals:
    void recipeCreated(const Dish &dish, const std::string &type);

public:
    explicit NewRecipe(QWidget *parent = nullptr) : QWidget(parent) {
        this->setGeometry(700, 700, 500, 500);
        this->setWindowTitle("New recipe");

        this->layout->addWidget(this->label1, 0, 0);
        this->layout->addWidget(this->nameLineEdit, 1, 0);
        this->layout->addWidget(this->label2, 2, 0);
        this->layout->addWidget(this->textForIngredients, 3, 0);
        this->layout->addWidget(this->label3, 4, 0);
        this->layout->addWidget(this->textForMaking, 5, 0);

        QString types[7] = {"appetizers", "main courses", "desserts", "beverages", "soups and stews", "salads",
                            "sandwiches"};

        for (QString &type : types) {
            this->options->addItem(type);
        }
        this->layout->addWidget(this->options, 6, 0);

        connect(this->button, &QPushButton::clicked, [this]() {
            Dish dish(this->nameLineEdit->text(), this->textForIngredients->toPlainText(),
                      this->textForMaking->toPlainText(),
                      this->data.get_available_id(this->options->currentText().toStdString()));
            emit recipeCreated(dish, this->options->currentText().toStdString());
            this->close();
        });
        this->layout->addWidget(this->button, 7, 0);

        this->setLayout(this->layout);
    }
};

class Recipe : public QWidget {
Q_OBJECT
signals:
    void deleteRecipeClicked(const Dish &_t1, const std::string &_t2);

public:
    Dish dish;
    std::string type;

    void onDeleteClicked() {
        emit deleteRecipeClicked(this->dish, this->type);
        this->close();
    }

    explicit Recipe(const Dish &dish, std::string type, QWidget *parent = nullptr)
            : QWidget(parent), dish(dish), type(std::move(type)) {
        this->setWindowTitle(dish.getName());
        this->setGeometry(700, 700, 300, 300);

        auto *layout = new QGridLayout;
        auto *label = new QLabel(dish.get_recipe());
        auto *button = new QPushButton("delete");
        connect(button, &QPushButton::clicked, this, &Recipe::onDeleteClicked);

        layout->addWidget(label, 0, 0);
        layout->addWidget(button, 1, 0);
        this->setLayout(layout);
    }
};

class Window : public QWidget {
Q_OBJECT
private:
    Data data;

    NewRecipe *newRecipe;
    std::vector<QPushButton *> listAppetizers;
    std::vector<QPushButton *> listMainCourses;
    std::vector<QPushButton *> listDesserts;
    std::vector<QPushButton *> listBeverages;
    std::vector<QPushButton *> listSoupsStews;
    std::vector<QPushButton *> listSalads;
    std::vector<QPushButton *> listSandwiches;

    QGridLayout *layout = new QGridLayout;

public:
    explicit Window(QWidget *parent = nullptr) : QWidget(parent) {
        this->setGeometry(100, 100, 700, 500);
        this->setWindowTitle("Recipes app");

        newRecipe = new NewRecipe();

        connect(newRecipe, &NewRecipe::recipeCreated, this, &Window::onRecipeCreated);
        this->createButtons();
        this->renderUI();

        this->setLayout(this->layout);
    }

    void createButtons() {
        listAppetizers.clear();
        listMainCourses.clear();
        listDesserts.clear();
        listBeverages.clear();
        listSoupsStews.clear();
        listSalads.clear();
        listSandwiches.clear();

        for (const Dish &dish : data.getAppetizers()) {
            auto *button = new QPushButton(dish.getName());
            connect(button, &QPushButton::clicked, [dish, this]() {
                auto *recipe = new Recipe(dish, "appetizers");
                connect(recipe, &Recipe::deleteRecipeClicked, this, &Window::onDeleteRecipeClicked);
                recipe->show();
            });
            this->listAppetizers.push_back(button);
        }

        for (const Dish &dish : data.getMainCourses()) {
            auto *button = new QPushButton(dish.getName());
            connect(button, &QPushButton::clicked, [dish, this]() {
                auto *recipe = new Recipe(dish, "main_courses");
                connect(recipe, &Recipe::deleteRecipeClicked, this, &Window::onDeleteRecipeClicked);
                recipe->show();
            });
            this->listMainCourses.push_back(button);
        }

        for (const Dish &dish : data.getDesserts()) {
            auto *button = new QPushButton(dish.getName());
            connect(button, &QPushButton::clicked, [dish, this]() {
                auto *recipe = new Recipe(dish, "desserts");
                connect(recipe, &Recipe::deleteRecipeClicked, this, &Window::onDeleteRecipeClicked);
                recipe->show();
            });
            this->listDesserts.push_back(button);
        }

        for (const Dish &dish : data.getBeverages()) {
            auto *button = new QPushButton(dish.getName());
            connect(button, &QPushButton::clicked, [dish, this]() {
                auto *recipe = new Recipe(dish, "beverages");
                connect(recipe, &Recipe::deleteRecipeClicked, this, &Window::onDeleteRecipeClicked);
                recipe->show();
            });
            this->listBeverages.push_back(button);
        }

        for (const Dish &dish : data.getSoupsStews()) {
            auto *button = new QPushButton(dish.getName());
            connect(button, &QPushButton::clicked, [dish, this]() {
                auto *recipe = new Recipe(dish, "soups_stews");
                connect(recipe, &Recipe::deleteRecipeClicked, this, &Window::onDeleteRecipeClicked);
                recipe->show();
            });
            this->listSoupsStews.push_back(button);
        }

        for (const Dish &dish : data.getSalads()) {
            auto *button = new QPushButton(dish.getName());
            connect(button, &QPushButton::clicked, [dish, this]() {
                auto *recipe = new Recipe(dish, "salads");
                connect(recipe, &Recipe::deleteRecipeClicked, this, &Window::onDeleteRecipeClicked);
                recipe->show();
            });
            this->listSalads.push_back(button);
        }

        for (const Dish &dish : data.getSandwiches()) {
            auto *button = new QPushButton(dish.getName());
            connect(button, &QPushButton::clicked, [dish, this]() {
                auto *recipe = new Recipe(dish, "sandwiches");
                connect(recipe, &Recipe::deleteRecipeClicked, this, &Window::onDeleteRecipeClicked);
                recipe->show();
            });
            this->listSandwiches.push_back(button);
        }
    }

    void renderUI() {
        QLayoutItem *item;
        while ((item = this->layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        auto *pushButton = new QPushButton("Add new item");
        connect(pushButton, &QPushButton::clicked, [this]() {
            newRecipe->show();
        });
        this->layout->addWidget(pushButton, 0, 3);

        QString types[7] = {"appetizers", "main courses", "desserts", "beverages", "soups and stews", "salads",
                            "sandwiches"};

        for (int i = 0; i < 7; i++) {
            auto label = new QLabel(types[i]);
            this->layout->addWidget(label, 1, i);
        }

        int row = 2;
        for (QPushButton *button : this->listAppetizers) {
            this->layout->addWidget(button, row, 0);
            row++;
        }

        row = 2;
        for (QPushButton *button : this->listMainCourses) {
            this->layout->addWidget(button, row, 1);
            row++;
        }

        row = 2;
        for (QPushButton *button : this->listDesserts) {
            this->layout->addWidget(button, row, 2);
            row++;
        }

        row = 2;
        for (QPushButton *button : this->listBeverages) {
            this->layout->addWidget(button, row, 3);
            row++;
        }

        row = 2;
        for (QPushButton *button : this->listSoupsStews) {
            this->layout->addWidget(button, row, 4);
            row++;
        }

        row = 2;
        for (QPushButton *button : this->listSalads) {
            this->layout->addWidget(button, row, 5);
            row++;
        }

        row = 2;
        for (QPushButton *button : this->listSandwiches) {
            this->layout->addWidget(button, row, 6);
            row++;
        }
    }

    void onDeleteRecipeClicked(const Dish &dish, const std::string &type) {
        this->data.delete_dish(dish.getId(), type);
        this->createButtons();
        this->renderUI();
    }

    void onRecipeCreated(const Dish &dish, const std::string &type) {
        this->data.addNewDish(dish, type);
        this->createButtons();
        this->renderUI();
    }
};

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    Window window;
    window.show();
    return QApplication::exec();
}

#include "main.moc"
