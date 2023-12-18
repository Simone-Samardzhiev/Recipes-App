#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <vector>

//path to the json file
const QString PATH = "/Users/simonesamardzhiev/Desktop/My projects/Recipes App/c++/data.json";

// Class that stores data of the dish
class Dish {
private:
    QString name;
    QString ingredients;
    QString cook;
    int id;
public:
    Dish(QString name, QString ingredients, QString cook, int i) {
        this->name = std::move(name);
        this->ingredients = std::move(ingredients);
        this->cook = std::move(cook);
        this->id = i;
    }

    // methods that return values
    const QString &getName() const {
        return name;
    }

    const QString &getIngredients() const {
        return ingredients;
    }

    const QString &getCook() const {
        return cook;
    }

    const QString get_recipe() const {
        return "Name : " + name + '\n' + "Ingredients : " + '\n' + ingredients + '\n' + "How ot do it : " + '\n' + cook;
    }

    int getId() const {
        return id;
    }

    // setter for the id
    void setId(int i) {
        Dish::id = i;
    }
};

// Class that stores the whole data
class Data {
private:
    // vectors for each type of dish
    std::vector<Dish> appetizers;
    std::vector<Dish> main_courses;
    std::vector<Dish> desserts;
    std::vector<Dish> beverages;
    std::vector<Dish> soups_stews;
    std::vector<Dish> salads;
    std::vector<Dish> sandwiches;
public:
    Data() {
        this->read_data();
    }

    // reading data method that parse the json object
    void read_data() {
        QFile file(PATH);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "There was an error opening the file for reading";
            return;
        }

        QString data = file.readAll();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject object = jsonDocument.object();

        read_dish(object, "appetizers", this->appetizers);
        read_dish(object, "main courses", this->main_courses);
        read_dish(object, "desserts", this->desserts);
        read_dish(object, "beverages", this->beverages);
        read_dish(object, "soups and stews", this->soups_stews);
        read_dish(object, "salads", this->salads);
        read_dish(object, "sandwiches", this->sandwiches);

        file.close();
    }

    // helper method that reads specific dish
    static void read_dish(const QJsonObject &object, const QString &key, std::vector<Dish> &dishes) {
        QJsonArray jsonArray = object[key].toArray();

        for (QJsonValue value: jsonArray) {
            QString name = value["name"].toString();
            QString ingredients = value["ingredients"].toString();
            QString cook = value["cook"].toString();
            int id = value["id"].toInt();
            dishes.emplace_back(name, ingredients, cook, id);
        }
    }

    // method that rewrite the data back to the json file
    void write_data() {
        QFile file(PATH);

        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "There was and error opening the file for writing";
            return;
        }

        QJsonObject object;
        object["appetizers"] = get_array(this->appetizers);
        object["main courses"] = get_array(this->main_courses);
        object["desserts"] = get_array(this->desserts);
        object["beverages"] = get_array(this->beverages);
        object["soups and stews"] = get_array(this->soups_stews);
        object["salads"] = get_array(this->salads);
        object["sandwiches"] = get_array(this->sandwiches);

        file.write(QJsonDocument(object).toJson());

        file.close();
    }

// helper function for writing data that return array of specific dishes
    static QJsonArray get_array(std::vector<Dish> &dishes) {
        QJsonArray array;

        for (const Dish &dish: dishes) {
            QJsonObject object;
            object["id"] = dish.getId();
            object["cook"] = dish.getCook();
            object["ingredients"] = dish.getIngredients();
            object["name"] = dish.getName();
            array.append(object);
        }

        return array;
    }

    // method that will update the indexes if each dish on exit
    void update_indexes() {
        reindexy_array(this->appetizers);
        reindexy_array(this->main_courses);
        reindexy_array(this->desserts);
        reindexy_array(this->beverages);
        reindexy_array(this->soups_stews);
        reindexy_array(this->salads);
        reindexy_array(this->sandwiches);
    }

    // helper method that will reindexy a specific dish
    static void reindexy_array(std::vector<Dish> &dishes) {
        int index = 0;

        for (Dish &dish: dishes) {
            dish.setId(index++);
        }
    }

    // method that will delete a dish based on type and id
    void delete_dish(int id, const std::string &type) {
        if (type == "appetizers") {
            this->appetizers.erase(this->appetizers.begin() + get_index(id, this->appetizers));
        } else if (type == "main_courses") {
            this->main_courses.erase(this->main_courses.begin() + get_index(id, this->main_courses));
        } else if (type == "desserts") {
            this->desserts.erase(this->desserts.begin() + get_index(id, this->desserts));
        } else if (type == "beverages") {
            this->beverages.erase(this->beverages.begin() + get_index(id, this->beverages));
        } else if (type == "soups_stews") {
            this->soups_stews.erase(this->soups_stews.begin() + get_index(id, this->soups_stews));
        } else if (type == "salads") {
            this->salads.erase(this->salads.begin() + get_index(id, this->salads));
        } else if (type == "sandwiches") {
            this->sandwiches.erase(this->sandwiches.begin() + get_index(id, this->sandwiches));
        } else {
            qDebug() << "Wrong type";
        }

        this->write_data();
    }

    // helper method that will update the index of specific dishes
    static int get_index(int id, std::vector<Dish> &dishes) {
        for (int i = 0; i < dishes.size(); i++) {
            if (dishes[i].getId() == id) {
                return i;
            }
        }
    }

    // method that will return available id based on type
    int get_available_id(const std::string &type) {
        if (type == "appetizers") {
            if (this->appetizers.empty()) {
                return 0;
            } else {
                return this->appetizers.back().getId() + 1;
            }
        } else if (type == "main courses") {
            if (this->main_courses.empty()) {
                return 0;
            } else {
                return this->main_courses.back().getId() + 1;
            }
        } else if (type == "desserts") {
            if (this->desserts.empty()) {
                return 0;
            } else {
                return this->desserts.back().getId() + 1;
            }
        } else if (type == "beverages") {
            if (this->beverages.empty()) {
                return 0;
            } else {
                return this->beverages.back().getId() + 1;
            }
        } else if (type == "soups and stews") {
            if (this->soups_stews.empty()) {
                return 0;
            } else {
                return this->soups_stews.back().getId() + 1;
            }
        } else if (type == "salads") {
            if (this->salads.empty()) {
                return 0;
            } else {
                return this->salads.back().getId() + 1;
            }
        } else if (type == "sandwiches") {
            if (this->sandwiches.empty()) {
                return 0;
            } else {
                return this->sandwiches.back().getId() + 1;
            }
        } else {
            qDebug() << "Wrong type";
        }
    }

    void addNewDish(const Dish &dish, const std::string &type) {
        if (type == "appetizers") {
            this->appetizers.push_back(dish);
        } else if (type == "main courses") {
            this->main_courses.push_back(dish);
        } else if (type == "desserts") {
            this->desserts.push_back(dish);
        } else if (type == "beverages") {
            this->beverages.push_back(dish);
        } else if (type == "soups and stews") {
            this->soups_stews.push_back(dish);
        } else if (type == "salads") {
            this->salads.push_back(dish);
        } else if (type == "sandwiches") {
            this->sandwiches.push_back(dish);
        } else {
            qDebug() << "Wrong type";
        }
        this->write_data();
    }

    const std::vector<Dish> &getAppetizers() const {
        return appetizers;
    }

    const std::vector<Dish> &getMainCourses() const {
        return main_courses;
    }

    const std::vector<Dish> &getDesserts() const {
        return desserts;
    }

    const std::vector<Dish> &getBeverages() const {
        return beverages;
    }

    const std::vector<Dish> &getSoupsStews() const {
        return soups_stews;
    }

    const std::vector<Dish> &getSalads() const {
        return salads;
    }

    const std::vector<Dish> &getSandwiches() const {
        return sandwiches;
    }

};



