import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.plaf.ComboBoxUI;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Map;
import java.util.Set;

class Recipe {
    private final String name;
    private final String way;

    private final String ingredients;

    private int index;

    public Recipe(String name, String way, String ingredients, int index) {
        this.name = name;
        this.way = way;
        this.ingredients = ingredients;
        this.index = index;
    }

    public String getName() {
        return name;
    }

    public String getWay() {
        return way;
    }

    public String getIngredients() {
        return ingredients;
    }

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }
}

class Data {
    private Map<String, ArrayList<Recipe>> recipes;

    public Data() {
        this.readData();
    }

    // function that will read and parse the json file
    private void readData() {
        try (FileReader reader = new FileReader("/home/simone/Desktop/My projects/Recipes App/java/data.json")) {
            Gson gson = new Gson();
            Type type = new TypeToken<Map<String, ArrayList<Recipe>>>() {
            }.getType();
            this.recipes = gson.fromJson(reader, type);
        } catch (IOException e) {
            System.err.println("There was an error in opening the file in readData");
        }
    }

    // function that will update the indexes of the recipes before writing them back to the json file
    private void reindex(ArrayList<Recipe> array) {
        for (int i = 0; i < array.size(); i++) {
            array.get(i).setIndex(i);
        }
    }

    // function that will write the data back to the json file
    public void writeData() {
        for (String key : this.recipes.keySet()) {
            reindex(this.recipes.get(key));
        }
        try (FileWriter writer = new FileWriter("/home/simone/Desktop/My projects/Recipes App/java/data.json")) {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            gson.toJson(this.recipes, writer);
        } catch (IOException e) {
            System.err.println("There was error in opening the file in writeData");
        }
    }

    // function that will delete a recipe based on the index and the key
    public void deleteRecipe(String key, int index) throws IllegalArgumentException {
        if (this.recipes.get(key) == null) {
            throw new IllegalArgumentException("The key wasn't found in the deleteRecipe");
        }

        for (int i = 0; i < this.recipes.get(key).size(); i++) {
            if (this.recipes.get(key).get(i).getIndex() == index) {
                this.recipes.get(key).remove(i);
                break;
            }
        }
    }

    // function that will return unique index for specific key
    public int getIndex(String key) throws IllegalArgumentException {
        try {
            if (this.recipes.get(key) == null) {
                throw new IllegalArgumentException("The key wasn't found in getIndex");
            } else {
                return this.recipes.get(key).get(this.recipes.get(key).size() - 1).getIndex() + 1;
            }
        } catch (IndexOutOfBoundsException e) {
            return 0;
        }
    }

    // function that will add a recipe to specific key
    void addNewRecipe(String key, Recipe recipe) throws IllegalArgumentException {
        if (this.recipes.get(key) == null) {
            throw new IllegalArgumentException("The key wasn't found in addNewRecipe");
        } else {
            this.recipes.get(key).add(recipe);
        }
    }

    // function that will return final set of keys
    public final Set<String> iterOverKeys() {
        return this.recipes.keySet();
    }

    // function that will return iterator for recipes based in the keys
    public final ArrayList<Recipe> iterOverRecipe(String key) throws IllegalArgumentException {
        if (this.recipes.get(key) == null) {
            throw new IllegalArgumentException("The key wasn't found in the iterOverRecipe");
        } else {
            return this.recipes.get(key);
        }
    }
}

class NewRecipeWindow extends JFrame {
    NewRecipeWindow(Data data, Window window) {
        this.setTitle("New recipe");
        this.setSize(400, 400);
        this.setLayout(new GridBagLayout());

        GridBagConstraints layout = new GridBagConstraints();
        layout.gridx = 0;
        layout.gridy = 0;

        TextField textForName = new TextField();
        textForName.setPreferredSize(new Dimension(100, 20));
        textForName.setText("Enter the name");
        this.add(textForName, layout);
        layout.gridy = 1;

        TextArea textForIngredients = new TextArea();
        textForIngredients.setText("Enter the ingredients");
        this.add(textForIngredients, layout);
        layout.gridy = 2;

        TextArea textForWay = new TextArea();
        textForWay.setText("Enter the way of making it");
        this.add(textForWay, layout);
        layout.gridy = 3;

        JComboBox<String> type = new JComboBox<>();

        for (String key : data.iterOverKeys()) {
            type.addItem(key);
        }
        this.add(type, layout);
        layout.gridy = 4;

        Button button = new Button("Add");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String key = (String) type.getSelectedItem();
                String name = textForName.getText();
                String ingredients = textForIngredients.getText();
                String way = textForWay.getText();
                int index = data.getIndex(key);
                Recipe recipe = new Recipe(name, way, ingredients, index);

                data.addNewRecipe(key, recipe);
                window.renderGUI();
                dispose();
            }
        });
        this.add(button, layout);

        this.setVisible(true);
    }
}

class PopUpRecipeInfo extends JFrame {
    public PopUpRecipeInfo(Data data, Window window, Recipe recipe, String key) {
        this.setSize(500, 500);
        this.setTitle(recipe.getName());

        JPanel panel = new JPanel(new GridLayout(4, 1));

        String way = recipe.getWay().replace("\n", "<br>");
        String ingredients = recipe.getIngredients().replace("\n", "<br>");

        JLabel nameLabel = new JLabel("Recipe Name: " + recipe.getName());
        JLabel wayLabel = new JLabel("<html> Way :<br> " + way + "</html>");
        JLabel ingredientsLabel = new JLabel("<html> Ingredients:<br> " + ingredients + "</html>");
        JButton button = new JButton("Delete");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                data.deleteRecipe(key, recipe.getIndex());
                window.renderGUI();
                dispose();
            }
        });

        panel.add(nameLabel);
        panel.add(wayLabel);
        panel.add(ingredientsLabel);
        panel.add(button);

        this.add(panel);
        this.pack();
        this.setVisible(true);
    }
}

class Window extends JFrame {
    private Data data = new Data();

    public Window() {
        super();

        this.setTitle("Recipes app");
        this.setSize(700, 700);
        this.setLayout(new GridBagLayout());
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                data.writeData();
                dispose();
                System.exit(0);
            }
        });

        this.renderGUI();

        this.setVisible(true);
    }

    // function that will render all the elements in the window
    public void renderGUI() {
        this.removeAllComponents();
        GridBagConstraints layout = new GridBagConstraints();
        layout.ipadx = 10;
        layout.ipady = 10;
        layout.gridy = 0;
        layout.gridx = 3;


        JButton buttonForNew = new JButton("Add new recipe");
        buttonForNew.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                new NewRecipeWindow(data, Window.this);
            }
        });
        this.add(buttonForNew, layout);

        layout.gridy = 1;
        layout.gridx = 0;

        for (String key : data.iterOverKeys()) {
            Border border = BorderFactory.createLineBorder(Color.BLACK, 2);

            JLabel label = new JLabel(key);

            label.setBorder(border);
            label.setHorizontalAlignment(SwingConstants.CENTER);

            this.add(label, layout);
            layout.gridx++;
        }

        layout.gridx = 0;

        for (String key : data.iterOverKeys()) {
            layout.gridy = 2;
            for (Recipe recipe : data.iterOverRecipe(key)) {
                JButton button = new JButton(recipe.getName());

                button.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        new PopUpRecipeInfo(data, Window.this, recipe, key);
                    }
                });

                this.add(button, layout);
                layout.gridy++;
            }
            layout.gridx++;
        }

        this.revalidate();
        this.repaint();
    }

    // function that will remove every item in the window
    private void removeAllComponents() {
        Component[] components = this.getContentPane().getComponents();
        for (Component component : components) {
            this.remove(component);
        }
        this.revalidate();
        this.repaint();
    }
}


public class Main {
    public static void main(String[] args) {
        Window wnd = new Window();
    }
}