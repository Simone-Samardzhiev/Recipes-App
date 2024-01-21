#include <vector>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValueConstRef>

#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QMouseEvent>
#include <QMessageBox>


class Recipe {
private:
    QString name;
    QString ingredients;
    QString instructions;
    QString type;
    int id;

public:
    Recipe(QString name, QString ingredients, QString instructions, QString type, int id)
            : name(std::move(name)), ingredients(std::move(ingredients)), instructions(std::move(instructions)),
              type(std::move(type)), id(id) {}

    const QString &getName() const {
        return name;
    }

    void setName(const QString &_name) {
        Recipe::name = _name;
    }

    const QString &getIngredients() const {
        return ingredients;
    }

    void setIngredients(const QString &_ingredients) {
        Recipe::ingredients = _ingredients;
    }

    const QString &getInstructions() const {
        return instructions;
    }

    void setInstructions(const QString &_instruction) {
        Recipe::instructions = _instruction;
    }

    const QString &getType() const {
        return type;
    }

    void setType(const QString &_type) {
        Recipe::type = _type;
    }

    int getId() const {
        return id;
    }

    void setId(int _id) {
        Recipe::id = _id;
    }
};

class Data {
private:
    std::vector<Recipe> recipes;
public:
    Data() {
        readData();
    }

private:
    void readData() {
        QFile file("/Users/simonesamardzhiev/Desktop/My projects/Recipes App/c++/data.json");
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Error in reading the data from the json file";
            exit(1);
        }
        QByteArray byteArray = file.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
        QJsonArray jsonArray = jsonDocument.array();

        for (const QJsonValueConstRef &value: jsonArray) {
            QJsonObject object = value.toObject();

            QString name = object.value("name").toString();
            QString ingredients = object.value("ingredients").toString();
            QString instructions = object.value("instructions").toString();
            QString type = object.value("type").toString();
            int id = object.value("id").toInt();

            recipes.emplace_back(name, ingredients, instructions, type, id);
        }
    }

    void revalidateIds() {
        for (int i = 0; i < recipes.size(); i++) {
            recipes[i].setId(i);
        }
    }

public:
    void writeData() {
        revalidateIds();

        QJsonArray jsonArray;
        for (const Recipe &recipe: recipes) {
            QJsonObject object;

            object["name"] = recipe.getName();
            object["ingredients"] = recipe.getIngredients();
            object["instructions"] = recipe.getInstructions();
            object["type"] = recipe.getType();
            object["id"] = recipe.getId();

            jsonArray.append(object);
        }
        QJsonDocument jsonDocument(jsonArray);
        QByteArray byteArray = jsonDocument.toJson();

        QFile file("/Users/simonesamardzhiev/Desktop/My projects/Recipes App/c++/data.json");

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Error in writing the data back to the json file";
            exit(1);
        }

        file.write(byteArray);
        file.close();
    }

private:
    int getNewId() {
        if (recipes.empty()) {
            return 0;
        } else {
            return recipes.back().getId() + 1;
        }
    }

public:
    void addRecipe(const QString &name, const QString &ingredients, const QString &instructions, const QString &type) {
        recipes.emplace_back(name, ingredients, instructions, type, getNewId());
    }

    void deleteRecipe(int id) {
        for (int i = 0; i < recipes.size(); i++) {
            if (recipes[i].getId() == id) {
                recipes.erase(recipes.begin() + i);
                break;
            }
        }
    }

    void changeRecipeName(const QString &name, int id) {
        for (Recipe &recipe: recipes) {
            if (recipe.getId() == id) {
                recipe.setName(name);
                break;
            }
        }
    }

    void changeRecipeIngredients(const QString &ingredients, int id) {
        for (Recipe &recipe: recipes) {
            if (recipe.getId() == id) {
                recipe.setIngredients(ingredients);
                break;
            }
        }
    }

    void changeRecipeInstructions(const QString &instructions, int id) {
        for (Recipe &recipe: recipes) {
            if (recipe.getId() == id) {
                recipe.setInstructions(instructions);
                break;
            }
        }
    }

    void changeRecipeType(const QString &type, int id) {
        for (Recipe &recipe: recipes) {
            if (recipe.getId() == id) {
                recipe.setType(type);
                break;
            }
        }
    }

    std::vector<Recipe>::const_iterator begin() const {
        return recipes.begin();
    }

    std::vector<Recipe>::const_iterator end() const {
        return recipes.end();
    }

    std::vector<Recipe>::iterator begin() {
        return recipes.begin();
    }

    std::vector<Recipe>::iterator end() {
        return recipes.end();
    }
};

class ReadOnlyCheckBox : public QCheckBox {
Q_OBJECT

public:
    explicit ReadOnlyCheckBox(QWidget *parent = nullptr) : QCheckBox(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        Q_UNUSED(event);
    }
};

class SetNameWindow : public QWidget {
Q_OBJECT

private:
    QLineEdit *nameLineEdit;
public:
    explicit SetNameWindow(const QString &name, QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("Set name");
        this->setGeometry(300, 300, 400, 50);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QHBoxLayout();
        nameLineEdit = new QLineEdit(name);
        auto setButton = new QPushButton("Set name");

        // setting attributes and connecting the widgets
        connect(setButton, &QPushButton::clicked, [=]() { onSetButtonClicked(); });

        // adding the widgets
        layout->addWidget(nameLineEdit);
        layout->addWidget(setButton);

        this->setLayout(layout);
        this->show();
    }

signals:

    void nameSet(const QString &name);

private  slots:

    void onSetButtonClicked() {
        emit nameSet(nameLineEdit->text());
        this->close();
    };
};

class SetIngredientsWindow : public QWidget {
Q_OBJECT

private:
    QTextEdit *ingredientsTextEdit;
public:
    explicit SetIngredientsWindow(const QString &ingredients, QWidget *parent = nullptr) : QWidget(nullptr) {
        // setting attributes to the window
        this->setWindowTitle("Set ingredients");
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QVBoxLayout();
        ingredientsTextEdit = new QTextEdit(ingredients);
        auto setButton = new QPushButton("Set ingredients");

        // setting attributes and connecting the widgets
        connect(setButton, &QPushButton::clicked, [=]() { onSetButtonClicked(); });

        // adding the widgets
        layout->addWidget(ingredientsTextEdit);
        layout->addWidget(setButton);

        this->setLayout(layout);
        this->show();
    }

signals:

    void ingredientsSet(const QString &ingredients);

private slots:

    void onSetButtonClicked() {
        emit ingredientsSet(ingredientsTextEdit->toPlainText());
        this->close();
    }
};

class SetInstructionsWindow : public QWidget {
Q_OBJECT

private:
    QTextEdit *instructionsTextEdit;
public:
    explicit SetInstructionsWindow(const QString &instructions, QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("Set instructions");
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QVBoxLayout();
        instructionsTextEdit = new QTextEdit(instructions);
        auto setButton = new QPushButton("Set instructions");

        // setting attributes and connecting the widgets
        connect(setButton, &QPushButton::clicked, [=]() { onSetButtonClicked(); });

        // adding the widgets
        layout->addWidget(instructionsTextEdit);
        layout->addWidget(setButton);

        this->setLayout(layout);
        this->show();
    }

signals:

    void instructionsSet(const QString &instructions);

private slots:

    void onSetButtonClicked() {
        emit instructionsSet(instructionsTextEdit->toPlainText());
        this->close();
    }
};

class SetTypeWindow : public QWidget {
Q_OBJECT

private:
    QComboBox *typeSelector;
public:
    explicit SetTypeWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("Set type");
        this->setGeometry(300, 300, 200, 200);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QVBoxLayout();
        typeSelector = new QComboBox();
        auto setButton = new QPushButton("Set type");

        // setting attributes and connecting the widgets
        QString types[] = {"Beverage", "Desserts", "Main courses", "Salads", "Sandwiches", "Soups and stews"};
        for (const QString &type: types) {
            typeSelector->addItem(type);
        }
        connect(setButton, &QPushButton::clicked, [=]() { onSetButtonClicked(); });

        // adding the widgets
        layout->addWidget(typeSelector);
        layout->addWidget(setButton);

        this->setLayout(layout);
        this->show();
    }

signals:

    void typeSet(const QString &type);

private slots:

    void onSetButtonClicked() {
        emit typeSet(typeSelector->currentText());
        this->close();
    }
};

class AddRecipeWindow : public QWidget {
Q_OBJECT

private:
    QString name;
    QString ingredients;
    QString instructions;
    QString type;

    ReadOnlyCheckBox *nameInputted;
    ReadOnlyCheckBox *ingredientsInputted;
    ReadOnlyCheckBox *instructionsInputted;
    ReadOnlyCheckBox *typeInputted;
public:
    explicit AddRecipeWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("Add recipe");
        this->setGeometry(300, 300, 200, 200);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QGridLayout();
        auto setNameButton = new QPushButton("Set name");
        auto setIngredientsButton = new QPushButton("Set ingredients");
        auto setInstructionsButton = new QPushButton("Set instructions");
        auto setTypeButton = new QPushButton("Set type");
        auto addRecipeButton = new QPushButton("Add recipe");

        nameInputted = new ReadOnlyCheckBox();
        ingredientsInputted = new ReadOnlyCheckBox();
        instructionsInputted = new ReadOnlyCheckBox();
        typeInputted = new ReadOnlyCheckBox();

        // setting attributes and connecting the widgets
        connect(setNameButton, &QPushButton::clicked, [=]() { onSetNameButtonClicked(); });
        connect(setIngredientsButton, &QPushButton::clicked, [=]() { onSetIngredientsButtonClicked(); });
        connect(setInstructionsButton, &QPushButton::clicked, [=]() { onSetInstructionButtonClicked(); });
        connect(setTypeButton, &QPushButton::clicked, [=]() { onSetTypeButtonClicked(); });
        connect(addRecipeButton, &QPushButton::clicked, [=]() { onAddRecipeButtonClicked(); });

        // adding the widgets
        layout->addWidget(setNameButton, 0, 0);
        layout->addWidget(nameInputted, 0, 1);
        layout->addWidget(setIngredientsButton, 1, 0);
        layout->addWidget(ingredientsInputted, 1, 1);
        layout->addWidget(setInstructionsButton, 2, 0);
        layout->addWidget(instructionsInputted, 2, 1);
        layout->addWidget(setTypeButton, 3, 0);
        layout->addWidget(typeInputted, 3, 1);
        layout->addWidget(addRecipeButton, 4, 0);

        this->setLayout(layout);
        this->show();
    }

private slots:

    void nameSet(const QString &_name) {
        this->name = _name;
        nameInputted->setChecked(true);
    };

    void onSetNameButtonClicked() {
        auto setNameWindow = new SetNameWindow(name);
        connect(setNameWindow, &SetNameWindow::nameSet, this, &AddRecipeWindow::nameSet);
    }


    void ingredientsSet(const QString &_ingredients) {
        this->ingredients = _ingredients;
        ingredientsInputted->setChecked(true);
    }

    void onSetIngredientsButtonClicked() {
        auto setIngredientsWindow = new SetIngredientsWindow(ingredients);
        connect(setIngredientsWindow, &SetIngredientsWindow::ingredientsSet, this, &AddRecipeWindow::ingredientsSet);
    }

    void instructionsSet(const QString &_instructions) {
        this->instructions = _instructions;
        instructionsInputted->setChecked(true);
    }

    void onSetInstructionButtonClicked() {
        auto setInstructionsWindow = new SetInstructionsWindow(instructions);
        connect(setInstructionsWindow, &SetInstructionsWindow::instructionsSet, this,
                &AddRecipeWindow::instructionsSet);
    }

    void typeSet(const QString &_type) {
        this->type = _type;
        this->typeInputted->setChecked(true);
    }

    void onSetTypeButtonClicked() {
        auto setTypeWindow = new SetTypeWindow();
        connect(setTypeWindow, &SetTypeWindow::typeSet, this, &AddRecipeWindow::typeSet);
    }

signals:

    void recipeAdded(const QString &name, const QString &ingredients, const QString &instructions, const QString &type);

private slots:

    void onAddRecipeButtonClicked() {
        if (!nameInputted->isChecked()) {
            QMessageBox::information(this, "Error", "You haven't entered the name");
            return;
        } else if (!ingredientsInputted->isChecked()) {
            QMessageBox::information(this, "Error", "You haven't entered the ingredients");
            return;
        } else if (!instructionsInputted->isChecked()) {
            QMessageBox::information(this, "Error", "You haven't entered the instructions");
            return;
        } else if (!typeInputted->isChecked()) {
            QMessageBox::information(this, "Error", "You haven't entered the type");
            return;
        }

        emit recipeAdded(name, ingredients, instructions, type);
        this->close();
    }
};

class ChangeNameWindow : public QWidget {
Q_OBJECT

private:
    QLineEdit *nameLineEdit;
public:
    explicit ChangeNameWindow(const QString &name, QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setGeometry(300, 300, 400, 50);
        this->setWindowTitle("Change name");
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating teh widgets
        auto layout = new QHBoxLayout();
        nameLineEdit = new QLineEdit(name);
        auto setButton = new QPushButton("Set name");

        // setting attributes and connecting the widgets
        connect(setButton, &QPushButton::clicked, [=]() { onSetButtonClicked(); });

        // adding the widgets
        layout->addWidget(nameLineEdit);
        layout->addWidget(setButton);

        this->setLayout(layout);
        this->show();
    }

signals:

    void nameChanged(const QString &name);

private slots:

    void onSetButtonClicked() {
        emit nameChanged(nameLineEdit->text());
        this->close();
    };
};

class ChangeIngredientsWindow : public QWidget {
Q_OBJECT

private:
    QTextEdit *ingredientsTextEdit;
public:
    explicit ChangeIngredientsWindow(const QString &ingredients, QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("Change ingredients");
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QVBoxLayout();
        ingredientsTextEdit = new QTextEdit(ingredients);
        auto setButton = new QPushButton("Set ingredients");

        // setting attributes and connecting teh widgets
        connect(setButton, &QPushButton::clicked, [=]() { onSetButtonClicked(); });

        // adding the widgets
        layout->addWidget(ingredientsTextEdit);
        layout->addWidget(setButton);

        this->setLayout(layout);
        this->show();
    }

signals:

    void ingredientsChanged(const QString &ingredients);

private slots:

    void onSetButtonClicked() {
        emit ingredientsChanged(ingredientsTextEdit->toPlainText());
        this->close();
    };
};

class ChangeInstructionsWindow : public QWidget {
Q_OBJECT

private:
    QTextEdit *instructionsTextEdit;
public:
    explicit ChangeInstructionsWindow(const QString &instructions, QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("Change instructions");
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QVBoxLayout();
        instructionsTextEdit = new QTextEdit(instructions);
        auto setButton = new QPushButton("Set instructions");

        // setting attributes and connecting the widgets
        connect(setButton, &QPushButton::clicked, [=]() { onSetButtonClicked(); });

        // adding the widgets
        layout->addWidget(instructionsTextEdit);
        layout->addWidget(setButton);

        this->setLayout(layout);
        this->show();
    }

signals:

    void instructionsChanged(const QString &instructions);

private slots:

    void onSetButtonClicked() {
        emit instructionsChanged(instructionsTextEdit->toPlainText());
        this->close();
    };
};

class ChangeTypeWindow : public QWidget {
Q_OBJECT

private:
    QComboBox *typeSelector;
public:
    explicit ChangeTypeWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle("Change type");
        this->setGeometry(300, 300, 100, 100);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QVBoxLayout();
        typeSelector = new QComboBox();
        auto setButton = new QPushButton("Set type");

        // setting attributes and connecting the widgets
        QString types[] = {"Beverage", "Desserts", "Main courses", "Salads", "Sandwiches", "Soups and stews"};
        for (const QString &type: types) {
            typeSelector->addItem(type);
        }
        connect(setButton, &QPushButton::clicked, [=]() { onSetButtonClicked(); });

        // adding the widgets
        layout->addWidget(typeSelector);
        layout->addWidget(setButton);

        this->setLayout(layout);
        this->show();
    }

signals:

    void typeChanged(const QString &type);

private slots:

    void onSetButtonClicked() {
        emit typeChanged(typeSelector->currentText());
        this->close();
    }
};

class RecipeInfoWindow : public QWidget {
Q_OBJECT

private:
    Data &data;
    Recipe &recipe;
    QLabel *ingredientsLabel;
    QLabel *instructionsLabel;
public:
    explicit RecipeInfoWindow(Recipe &recipe, Data &data, QWidget *parent = nullptr) : recipe(recipe), data(data),
                                                                                       QWidget(parent) {
        // setting attributes to the window
        this->setWindowTitle(QString("Name : %1 , Type : %2").arg(recipe.getName()).arg(recipe.getType()));
        this->setGeometry(300, 300, 500, 500);
        this->setAttribute(Qt::WA_DeleteOnClose);

        // creating the widgets
        auto layout = new QGridLayout();
        auto labelsLayout = new QHBoxLayout();

        ingredientsLabel = new QLabel("Ingredients :\n" + recipe.getIngredients());
        instructionsLabel = new QLabel("Instructions :\n" + recipe.getInstructions());

        auto labelsFrame = new QFrame();
        auto ingredientsLabelScrollArea = new QScrollArea();
        auto instructionLabelScrollArea = new QScrollArea();

        auto changeNameButton = new QPushButton("Change name");
        auto changeIngredientsButton = new QPushButton("Change ingredients");
        auto changeInstructionsButton = new QPushButton("Change instructions");
        auto changeTypeButton = new QPushButton("Change type");
        auto deleteButton = new QPushButton("Delete");

        // setting attributes and connecting the widgets
        labelsFrame->setLayout(labelsLayout);

        ingredientsLabel->setStyleSheet("QLabel { padding-left: 20px; }");
        instructionsLabel->setStyleSheet("QLabel { padding-left: 20px; }");

        ingredientsLabelScrollArea->setWidget(ingredientsLabel);
        ingredientsLabelScrollArea->setWidgetResizable(true);

        instructionLabelScrollArea->setWidget(instructionsLabel);
        instructionLabelScrollArea->setWidgetResizable(true);

        connect(changeNameButton, &QPushButton::clicked, [=]() { onChangeNameButtonClicked(); });
        connect(changeIngredientsButton, &QPushButton::clicked, [=]() { onChangeIngredientsButtonClicked(); });
        connect(changeInstructionsButton, &QPushButton::clicked, [=]() { onChangeInstructionsButtonClicked(); });
        connect(changeTypeButton, &QPushButton::clicked, [=]() { onChangeTypeButtonClicked(); });
        connect(deleteButton, &QPushButton::clicked, [=]() { onDeleteButtonClicked(); });

        // adding the widgets
        labelsLayout->addWidget(ingredientsLabelScrollArea);
        labelsLayout->addWidget(instructionLabelScrollArea);

        labelsLayout->setStretch(0, 1);
        labelsLayout->setStretch(1, 1);

        layout->addWidget(changeNameButton, 0, 0);
        layout->addWidget(changeIngredientsButton, 0, 1);
        layout->addWidget(changeInstructionsButton, 0, 2);
        layout->addWidget(changeTypeButton, 0, 3);
        layout->addWidget(deleteButton, 0, 4);
        layout->addWidget(labelsFrame, 1, 0, 1, 5);

        this->setLayout(layout);
        this->show();
    }

signals:

    void changedRecipe();

private slots:

    void changeTitle() {
        this->setWindowTitle(QString("Name : %1 , Type : %2").arg(recipe.getName()).arg(recipe.getType()));
    }

    void nameChanged(const QString &name) {
        data.changeRecipeName(name, recipe.getId());
        changeTitle();
        emit changedRecipe();
    }

    void onChangeNameButtonClicked() {
        auto changeNameWindow = new ChangeNameWindow(recipe.getName());
        connect(changeNameWindow, &ChangeNameWindow::nameChanged, this, &RecipeInfoWindow::nameChanged);
    }

    void ingredientsChanged(const QString &ingredients) {
        data.changeRecipeIngredients(ingredients, recipe.getId());
        ingredientsLabel->setText("Ingredients :\n" + recipe.getIngredients());
        emit changedRecipe();

    }

    void onChangeIngredientsButtonClicked() {
        auto changeIngredientsWindow = new ChangeIngredientsWindow(recipe.getIngredients());
        connect(changeIngredientsWindow, &ChangeIngredientsWindow::ingredientsChanged, this,
                &RecipeInfoWindow::ingredientsChanged);
    }

    void instructionsChanged(const QString &instructions) {
        data.changeRecipeInstructions(instructions, recipe.getId());
        instructionsLabel->setText("Instructions :\n" + recipe.getInstructions());
        emit changedRecipe();
    }

    void onChangeInstructionsButtonClicked() {
        auto changeInstructionsWindow = new ChangeInstructionsWindow(recipe.getInstructions());
        connect(changeInstructionsWindow, &ChangeInstructionsWindow::instructionsChanged, this,
                &RecipeInfoWindow::instructionsChanged);
    }

    void typeChanged(const QString &type) {
        data.changeRecipeType(type, recipe.getId());
        changeTitle();
        emit changedRecipe();
    }

    void onChangeTypeButtonClicked() {
        auto changeTypeWindow = new ChangeTypeWindow();
        connect(changeTypeWindow, &ChangeTypeWindow::typeChanged, this, &RecipeInfoWindow::typeChanged);
    }

    void onDeleteButtonClicked() {
        data.deleteRecipe(recipe.getId());
        emit changedRecipe();
        this->close();
    }
};

class Window : public QWidget {
Q_OBJECT

private:
    Data data;
    QLineEdit *searchBar;
    QComboBox *typeSelector;
    QVBoxLayout *resultsLayout;
public:
    explicit Window(QWidget *parent = nullptr) : QWidget(parent) {
        // setting attributes to the widow
        this->setWindowTitle("Recipe app");
        this->setGeometry(300, 300, 500, 500);

        // creating the widgets
        auto layout = new QGridLayout();
        resultsLayout = new QVBoxLayout();

        auto resultsFrame = new QFrame();
        auto resultsScrollArea = new QScrollArea();

        searchBar = new QLineEdit();
        typeSelector = new QComboBox();

        auto addButton = new QPushButton("Add recipe");

        // setting attributes and connecting the widgets
        resultsFrame->setLayout(resultsLayout);

        resultsScrollArea->setWidget(resultsFrame);
        resultsScrollArea->setWidgetResizable(true);

        connect(searchBar, &QLineEdit::returnPressed, [=]() { onSearch(); });
        QString types[] = {"All", "Beverage", "Desserts", "Main courses", "Salads", "Sandwiches", "Soups and stews"};
        for (const QString &type: types) {
            typeSelector->addItem(type);
        }
        connect(addButton, &QPushButton::clicked, [=]() { onAddButtonClicked(); });

        // adding the widgets
        layout->addWidget(searchBar, 0, 0);
        layout->addWidget(typeSelector, 0, 1);
        layout->addWidget(resultsScrollArea, 1, 0, 1, 2);
        layout->addWidget(addButton, 2, 0, 1, 2);

        this->setLayout(layout);
    }

public slots:

    void onSearch() {
        QLayoutItem *child;
        while ((child = resultsLayout->takeAt(0)) != nullptr) {
            QWidget *widget = child->widget();
            if (widget != nullptr) {
                resultsLayout->removeWidget(widget);
                delete widget;
            }
            delete child;
        }

        QString text = searchBar->text().trimmed().toLower();
        QString type = typeSelector->currentText();

        if (type == "All") {
            for (Recipe &recipe: data) {
                if (recipe.getName().contains(text)) {
                    auto result = new QPushButton(recipe.getName());
                    connect(result, &QPushButton::clicked, [&]() { onResultClicked(recipe); });
                    resultsLayout->addWidget(result);
                }
            }
        } else {
            for (Recipe &recipe: data) {
                if (recipe.getName().contains(text) && recipe.getType() == type) {
                    auto result = new QPushButton(recipe.getName());
                    connect(result, &QPushButton::clicked, [&]() { onResultClicked(recipe); });
                    resultsLayout->addWidget(result);
                }
            }
        }
    }

private slots:

    void
    recipeAdded(const QString &name, const QString &ingredients, const QString &instructions, const QString &type) {
        data.addRecipe(name, instructions, instructions, type);
        onSearch();
    }

    void onAddButtonClicked() {
        auto addRecipeWindow = new AddRecipeWindow();
        connect(addRecipeWindow, &AddRecipeWindow::recipeAdded, this, &Window::recipeAdded);
    }

    void onResultClicked(Recipe &recipe) {
        auto resultWindowInfo = new RecipeInfoWindow(recipe, data);
        connect(resultWindowInfo, &RecipeInfoWindow::changedRecipe, this, &Window::onSearch);
    }

protected:
    void closeEvent(QCloseEvent *closeEvent) override {
        data.writeData();
        closeEvent->accept();
    }
};

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    Window window;
    window.show();
    return QApplication::exec();
}

#include "main.moc"