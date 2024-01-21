import com.google.gson.Gson;

import com.google.gson.reflect.TypeToken;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.Type;
import java.sql.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Objects;

class Recipe {
    private String name;
    private String ingredients;
    private String wayOfMaking;
    private String type;
    private final int id;

    public Recipe(String name, String ingredients, String wayOfMaking, String type, int id) {
        this.name = name;
        this.ingredients = ingredients;
        this.wayOfMaking = wayOfMaking;
        this.type = type;
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getIngredients() {
        return ingredients;
    }

    public void setIngredients(String ingredients) {
        this.ingredients = ingredients;
    }

    public String getWayOfMaking() {
        return wayOfMaking;
    }

    public void setWayOfMaking(String wayOfMaking) {
        this.wayOfMaking = wayOfMaking;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public int getId() {
        return id;
    }

}

class Data implements Iterable<Recipe> {
    private final ArrayList<Recipe> recipes = new ArrayList<>();
    private String username;
    private String password;

    public Data() {
        getLoginInfo();
        retrieveData();
    }

    private void getLoginInfo() {
        try (FileReader fileReader = new FileReader("/Users/simonesamardzhiev/Desktop/My projects/Recipes App/java/login_info.json")) {
            Gson gson = new Gson();
            Type type = new TypeToken<HashMap<String, String>>() {
            }.getType();
            HashMap<String, String> loginInfo = gson.fromJson(fileReader, type);

            username = loginInfo.get("username");
            password = loginInfo.get("password");
        } catch (IOException e) {
            System.err.println("There was an error reading the login info ");
            System.exit(1);
        }
    }

    private void retrieveData() {
        String url = "jdbc:mysql://localhost:3306/RecipeApp";

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection connection = DriverManager.getConnection(url, username, password);
            Statement statement = connection.createStatement();
            String query = "SELECT * FROM JavaData";
            ResultSet resultSet = statement.executeQuery(query);

            while (resultSet.next()) {
                String name = resultSet.getString("name");
                String ingredients = resultSet.getString("ingredients");
                String wayOfMaking = resultSet.getString("wayOfMaking");
                String type = resultSet.getString("type");
                int id = resultSet.getInt("id");

                recipes.add(new Recipe(name, ingredients, wayOfMaking, type, id));
            }

            connection.close();
            statement.close();
            resultSet.close();

        } catch (Exception e) {
            System.err.println("Error in retrieving data");
            System.exit(1);
        }
    }

    public void saveData() {
        String url = "jdbc:mysql://localhost:3306/RecipeApp";
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection connection = DriverManager.getConnection(url, username, password);
            Statement statement = connection.createStatement();
            String query = "DELETE FROM JavaData";
            statement.executeUpdate(query);
            PreparedStatement preparedStatement = connection.prepareStatement("INSERT INTO JavaData(name, ingredients, wayOfMaking, type) VALUES (?,?,?,?)");

            for (Recipe recipe : recipes) {
                preparedStatement.setString(1, recipe.getName());
                preparedStatement.setString(2, recipe.getIngredients());
                preparedStatement.setString(3, recipe.getWayOfMaking());
                preparedStatement.setString(4, recipe.getType());
                preparedStatement.executeUpdate();
            }

            connection.close();
            statement.close();
            preparedStatement.close();
        } catch (Exception e) {
            System.err.println("There was an error in saving the data");
            System.exit(0);
        }
    }

    public void deleteRecipe(int id) {
        for (Recipe recipe : recipes) {
            if (recipe.getId() == id) {
                recipes.remove(recipe);
                break;
            }
        }
    }

    public void addRecipe(String name, String ingredients, String wayOfMaking, String type) {
        recipes.add(new Recipe(name, ingredients, wayOfMaking, type, getNewId()));
    }

    public void changeRecipeName(String name, int id) {
        for (Recipe recipe : recipes) {
            if (recipe.getId() == id) {
                recipe.setName(name);
                break;
            }
        }
    }

    public void changeRecipeIngredients(String ingredients, int id) {
        for (Recipe recipe : recipes) {
            if (recipe.getId() == id) {
                recipe.setIngredients(ingredients);
                break;
            }
        }
    }


    public void changeRecipeWayOfMaking(String wayOfMaking, int id) {
        for (Recipe recipe : recipes) {
            if (recipe.getId() == id) {
                recipe.setWayOfMaking(wayOfMaking);
                break;
            }
        }
    }

    public void changeRecipeType(String type, int id) {
        for (Recipe recipe : recipes) {
            if (recipe.getId() == id) {
                recipe.setType(type);
                break;
            }
        }
    }


    private int getNewId() {
        if (recipes.isEmpty()) {
            return 0;
        } else {
            return recipes.getLast().getId() + 1;
        }
    }

    @Override
    public Iterator<Recipe> iterator() {
        return recipes.iterator();
    }
}


class SetNameWindow extends JFrame {
    private final AddRecipeWindow addRecipeWindow;
    private final JTextField textField;

    public SetNameWindow(AddRecipeWindow addRecipeWindow) {
        super();

        // setting attributes to the window
        this.setSize(500, 50);
        this.setTitle("Set name");
        this.setLayout(new GridLayout(1, 3));

        // passing the arguments
        this.addRecipeWindow = addRecipeWindow;

        // creating the widgets
        JLabel label = new JLabel("Name :");
        textField = new JTextField();
        JButton buttonSet = new JButton("Set");

        // setting attributes and connecting the widgets
        label.setHorizontalAlignment(SwingConstants.CENTER);
        buttonSet.addActionListener(e -> SetNameWindow.this.onSetButtonClicked());
        // adding the widgets
        this.add(label);
        this.add(textField);
        this.add(buttonSet);

        this.setVisible(true);
    }

    private void onSetButtonClicked() {
        String name = textField.getText();
        addRecipeWindow.onNameSet(name);
        this.dispose();
    }

}

class SetIngredientsWindow extends JFrame {
    private final AddRecipeWindow addRecipeWindow;
    private final JTextArea textArea;

    public SetIngredientsWindow(AddRecipeWindow addRecipeWindow) {
        super();

        // setting attributes to the window
        this.setSize(500, 500);
        this.setTitle("Set ingredients");
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.addRecipeWindow = addRecipeWindow;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        textArea = new JTextArea();
        JScrollPane scrollPane = new JScrollPane(textArea);
        JButton buttonSet = new JButton("Set");

        // setting attributes and connecting the widgets
        buttonSet.addActionListener(e -> SetIngredientsWindow.this.onSetButtonClicked());
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);


        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        layout.fill = GridBagConstraints.BOTH;
        layout.weightx = 1;
        layout.weighty = 0.95;
        this.add(scrollPane, layout);

        layout.gridy = 1;
        layout.weighty = 0.05;
        this.add(buttonSet, layout);

        this.setVisible(true);
    }

    private void onSetButtonClicked() {
        addRecipeWindow.onIngredientsSet(textArea.getText());
        this.dispose();
    }
}

class SetWayOfMakingWindow extends JFrame {
    private final AddRecipeWindow addRecipeWindow;
    private final JTextArea textArea;

    public SetWayOfMakingWindow(AddRecipeWindow addRecipeWindow) {
        super();

        // setting attributes to the window
        this.setSize(500, 500);
        this.setTitle("Set way of making");
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.addRecipeWindow = addRecipeWindow;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        textArea = new JTextArea();
        JScrollPane scrollPane = new JScrollPane(textArea);
        JButton buttonSet = new JButton("Set");

        // setting attributes and connecting the widgets
        buttonSet.addActionListener(e -> SetWayOfMakingWindow.this.onSetButtonClicked());
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);


        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        layout.fill = GridBagConstraints.BOTH;
        layout.weightx = 1;
        layout.weighty = 0.95;
        this.add(scrollPane, layout);

        layout.gridy = 1;
        layout.weighty = 0.05;
        this.add(buttonSet, layout);

        this.setVisible(true);
    }

    private void onSetButtonClicked() {
        addRecipeWindow.onWayOfMakingSet(textArea.getText());
        this.dispose();
    }

}

class SetTypeWindow extends JFrame {
    private final AddRecipeWindow addRecipeWindow;
    private final JComboBox<String> types;

    public SetTypeWindow(AddRecipeWindow addRecipeWindow) {
        super();

        // setting attributes to the window
        this.setTitle("Change type");
        this.setSize(150, 150);
        this.setLayout(new GridLayout(2, 1));

        // passing the arguments
        this.addRecipeWindow = addRecipeWindow;


        // creating the widgets
        types = new JComboBox<>();
        JButton buttonSet = new JButton("Set");

        // setting attributes and connecting the widgets
        String[] _types = {"appetizers", "beverages", "main courses", "salads", "sandwiches", "soups and stews"};
        for (String type : _types) {
            types.addItem(type);
        }
        buttonSet.addActionListener(e -> SetTypeWindow.this.onSetButtonClicked());

        // adding the widgets
        this.add(types);
        this.add(buttonSet);

        this.setVisible(true);
    }

    private void onSetButtonClicked() {
        addRecipeWindow.onTypeSet((String) types.getSelectedItem());
        this.dispose();
    }
}

class AddRecipeWindow extends JFrame {
    private final Window window;
    private final Data data;
    private final JLabel nameSet;
    private final JLabel ingredientsSet;
    private final JLabel wayOfMakingSet;
    private final JLabel typeSet;
    private String name;
    private String ingredients;
    private String wayOfMaking;
    private String type;

    private final boolean[] inputted = {false, false, false, false};


    public AddRecipeWindow(Window window, Data data) {
        super();

        // setting arguments to the window
        this.setSize(350, 350);
        this.setTitle("New Recipe");
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.window = window;
        this.data = data;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        JButton setNameButton = new JButton("Set name");
        nameSet = new JLabel("");
        JButton setIngredientsButton = new JButton("Set ingredients");
        ingredientsSet = new JLabel();
        JButton setWayOfMakingButton = new JButton("Set way of making");
        wayOfMakingSet = new JLabel();
        JButton setTypeButton = new JButton("Set type");
        typeSet = new JLabel();
        JButton addRecipeButton = new JButton("Add recipe");

        // setting attributes and connecting the widgets
        ImageIcon icon = new ImageIcon("/Users/simonesamardzhiev/Desktop/My projects/Recipes App/java/check box red.png");

        setNameButton.addActionListener(e -> new SetNameWindow(AddRecipeWindow.this));
        nameSet.setIcon(icon);

        setIngredientsButton.addActionListener(e -> new SetIngredientsWindow(AddRecipeWindow.this));
        ingredientsSet.setIcon(icon);

        setWayOfMakingButton.addActionListener(e -> new SetWayOfMakingWindow(AddRecipeWindow.this));
        wayOfMakingSet.setIcon(icon);

        setTypeButton.addActionListener(e -> new SetTypeWindow(AddRecipeWindow.this));
        typeSet.setIcon(icon);

        addRecipeButton.addActionListener(e -> AddRecipeWindow.this.onAddRecipeButtonClicked());

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        layout.weightx = 1;
        layout.fill = GridBagConstraints.BOTH;
        this.add(setNameButton, layout);

        layout.gridx = 1;
        this.add(nameSet, layout);

        layout.gridx = 0;
        layout.gridy = 1;
        this.add(setIngredientsButton, layout);

        layout.gridx = 1;
        this.add(ingredientsSet, layout);

        layout.gridx = 0;
        layout.gridy = 2;
        this.add(setWayOfMakingButton, layout);

        layout.gridx = 1;
        this.add(wayOfMakingSet, layout);

        layout.gridx = 0;
        layout.gridy = 3;
        this.add(setTypeButton, layout);

        layout.gridx = 1;
        this.add(typeSet, layout);

        layout.gridx = 0;
        layout.gridy = 4;
        this.add(addRecipeButton, layout);

        this.setVisible(true);
    }

    public void onNameSet(String name) {
        ImageIcon icon = new ImageIcon("/Users/simonesamardzhiev/Desktop/My projects/Recipes App/java/check box green.png");
        this.name = name;
        nameSet.setIcon(icon);
        inputted[0] = true;
    }

    public void onIngredientsSet(String ingredients) {
        ImageIcon icon = new ImageIcon("/Users/simonesamardzhiev/Desktop/My projects/Recipes App/java/check box green.png");
        this.ingredients = ingredients;
        ingredientsSet.setIcon(icon);
        inputted[1] = true;
    }

    public void onWayOfMakingSet(String wayOfMaking) {
        ImageIcon icon = new ImageIcon("/Users/simonesamardzhiev/Desktop/My projects/Recipes App/java/check box green.png");
        this.wayOfMaking = wayOfMaking;
        wayOfMakingSet.setIcon(icon);
        inputted[2] = true;
    }

    public void onTypeSet(String type) {
        ImageIcon icon = new ImageIcon("/Users/simonesamardzhiev/Desktop/My projects/Recipes App/java/check box green.png");
        this.type = type;
        typeSet.setIcon(icon);
        inputted[3] = true;
    }

    private void onAddRecipeButtonClicked() {
        if (!inputted[0]) {
            JOptionPane.showMessageDialog(this, "The name is not set !");
            return;
        } else if (!inputted[1]) {
            JOptionPane.showMessageDialog(this, "The ingredients are not set !");
            return;
        } else if (!inputted[2]) {
            JOptionPane.showMessageDialog(this, "The way of making is not set !");
            return;
        } else if (!inputted[3]) {
            JOptionPane.showMessageDialog(this, "The type is not set !");
            return;
        }

        data.addRecipe(name, ingredients, wayOfMaking, type);
        window.onSearch();
        this.dispose();
    }
}

class ChangeNameWindow extends JFrame {
    private final Window window;
    private final RecipeWindowInfo recipeWindowInfo;
    private final Data data;
    private final Recipe recipe;

    private final JTextField textField;

    public ChangeNameWindow(Window window, RecipeWindowInfo recipeWindowInfo, Data data, Recipe recipe) {
        super();

        // setting attributes to the window
        this.setTitle("Change name");
        this.setSize(500, 50);
        this.setLayout(new GridLayout(1, 3));
        this.setResizable(false);


        // passing the arguments
        this.window = window;
        this.recipeWindowInfo = recipeWindowInfo;
        this.data = data;
        this.recipe = recipe;

        // creating the widgets
        JLabel label = new JLabel("Name :");
        textField = new JTextField(recipe.getName());
        JButton saveButton = new JButton("Save");

        // setting attributes and connecting the widgets
        label.setHorizontalAlignment(SwingConstants.CENTER);
        saveButton.addActionListener(e -> ChangeNameWindow.this.onSaveButtonClicked());

        // adding the widgets
        this.add(label);
        this.add(textField);
        this.add(saveButton);

        this.setVisible(true);

    }

    private void onSaveButtonClicked() {
        data.changeRecipeName(textField.getText(), recipe.getId());
        window.onSearch();
        recipeWindowInfo.onNameChanged();
        this.dispose();
    }
}

class ChangeIngredientsWindow extends JFrame {
    private final Window window;
    private final RecipeWindowInfo recipeWindowInfo;
    private final Data data;
    private final Recipe recipe;
    private final JTextArea textArea;

    public ChangeIngredientsWindow(Window window, RecipeWindowInfo recipeWindowInfo, Data data, Recipe recipe) {
        super();

        // setting window attributes
        this.setTitle("Change Ingredients");
        this.setSize(500, 500);
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.window = window;
        this.recipeWindowInfo = recipeWindowInfo;
        this.data = data;
        this.recipe = recipe;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        textArea = new JTextArea(recipe.getIngredients());
        JScrollPane scrollPane = new JScrollPane(textArea);
        JButton saveButton = new JButton("Save");

        // setting attributes and connecting the widgets
        saveButton.addActionListener(e -> ChangeIngredientsWindow.this.onSaveButtonClicked());

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        layout.fill = GridBagConstraints.BOTH;
        layout.weighty = 0.95;
        layout.weightx = 1;
        this.add(scrollPane, layout);

        layout.gridy = 1;
        layout.weighty = 0.05;
        this.add(saveButton, layout);

        this.setVisible(true);
    }

    private void onSaveButtonClicked() {
        data.changeRecipeIngredients(textArea.getText(), recipe.getId());
        recipeWindowInfo.renderText();
        window.onSearch();
        this.dispose();
    }
}

class ChangeWayOfMakingWindow extends JFrame {
    private final Window window;
    private final RecipeWindowInfo recipeWindowInfo;
    private final Data data;
    private final Recipe recipe;
    private final JTextArea textArea;

    public ChangeWayOfMakingWindow(Window window, RecipeWindowInfo recipeWindowInfo, Data data, Recipe recipe) {
        super();

        // setting window attributes
        this.setTitle("Change Ingredients");
        this.setSize(500, 500);
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.window = window;
        this.recipeWindowInfo = recipeWindowInfo;
        this.data = data;
        this.recipe = recipe;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        textArea = new JTextArea(recipe.getWayOfMaking());
        JScrollPane scrollPane = new JScrollPane(textArea);
        JButton saveButton = new JButton("Save");

        // setting attributes and connecting the widgets
        saveButton.addActionListener(e -> ChangeWayOfMakingWindow.this.onSaveButtonClicked());

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        layout.fill = GridBagConstraints.BOTH;
        layout.weighty = 0.95;
        layout.weightx = 1;
        this.add(scrollPane, layout);

        layout.gridy = 1;
        layout.weighty = 0.05;
        this.add(saveButton, layout);

        this.setVisible(true);
    }

    private void onSaveButtonClicked() {
        data.changeRecipeWayOfMaking(textArea.getText(), recipe.getId());
        recipeWindowInfo.renderText();
        window.onSearch();
        this.dispose();
    }
}

class ChangeTypeWindow extends JFrame {
    private final Window window;
    private final Data data;
    private final Recipe recipe;

    private final JComboBox<String> types;

    public ChangeTypeWindow(Window window, Data data, Recipe recipe) {
        super();

        // setting attributes to the window
        this.setTitle("Change type");
        this.setSize(150, 150);
        this.setLayout(new GridLayout(2, 1));

        // passing the arguments
        this.window = window;
        this.data = data;
        this.recipe = recipe;

        // creating the widgets
        types = new JComboBox<>();
        JButton saveButton = new JButton("Save");

        // setting attributes and connecting the widgets
        String[] _types = {"appetizers", "beverages", "main courses", "salads", "sandwiches", "soups and stews"};
        for (String type : _types) {
            types.addItem(type);
        }
        saveButton.addActionListener(e -> ChangeTypeWindow.this.onSaveButtonClicked());

        // adding the widgets
        this.add(types);
        this.add(saveButton);

        this.setVisible(true);
    }

    private void onSaveButtonClicked() {
        String type = (String) types.getSelectedItem();
        data.changeRecipeType(type, recipe.getId());
        window.onSearch();
        this.dispose();
    }

}

class RecipeWindowInfo extends JFrame {
    private final Window window;
    private final Data data;
    private final Recipe recipe;
    private final JLabel ingredientsLabel;
    private final JLabel wayOfMakingLabel;

    public RecipeWindowInfo(Window window, Data data, Recipe recipe) {
        super();

        // setting attributes to the window
        this.setTitle(recipe.getName());
        this.setSize(900, 750);
        this.setLayout(new GridBagLayout());

        // passing the arguments
        this.window = window;
        this.data = data;
        this.recipe = recipe;

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        JPanel buttonHolder = new JPanel();
        JButton changeNameButton = new JButton("Change name");
        JButton changeIngredientsButton = new JButton("Change ingredients");
        JButton changeWayOfMakingButton = new JButton("Change the way of making");
        JButton changeTypeButton = new JButton("Change type");
        JButton deleteButton = new JButton("Delete");

        JPanel scrollPaneHolder = new JPanel();
        ingredientsLabel = new JLabel();
        JScrollPane ingredientsScrollPane = new JScrollPane(ingredientsLabel);
        wayOfMakingLabel = new JLabel();
        JScrollPane wayOfMakingScrollPane = new JScrollPane(wayOfMakingLabel);

        // setting attributes and connecting the widgets
        buttonHolder.setLayout(new GridLayout(1, 5));

        changeNameButton.addActionListener(e -> new ChangeNameWindow(
                RecipeWindowInfo.this.window,
                RecipeWindowInfo.this,
                RecipeWindowInfo.this.data,
                recipe));

        changeIngredientsButton.addActionListener(e -> new ChangeIngredientsWindow(
                RecipeWindowInfo.this.window,
                RecipeWindowInfo.this,
                RecipeWindowInfo.this.data,
                recipe));

        changeWayOfMakingButton.addActionListener(e -> new ChangeWayOfMakingWindow(
                RecipeWindowInfo.this.window,
                RecipeWindowInfo.this,
                RecipeWindowInfo.this.data,
                recipe));

        changeTypeButton.addActionListener(e -> new ChangeTypeWindow(RecipeWindowInfo.this.window,
                RecipeWindowInfo.this.data,
                RecipeWindowInfo.this.recipe));

        deleteButton.addActionListener(e -> RecipeWindowInfo.this.onDeleteButtonClicked());

        ingredientsScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        ingredientsScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        ingredientsScrollPane.setBorder(null);

        wayOfMakingScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        wayOfMakingScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        scrollPaneHolder.setLayout(new GridLayout(1, 2));
        wayOfMakingScrollPane.setBorder(null);

        renderText();

        // adding the widgets
        buttonHolder.add(changeNameButton);
        buttonHolder.add(changeIngredientsButton);
        buttonHolder.add(changeWayOfMakingButton);
        buttonHolder.add(changeTypeButton);
        buttonHolder.add(deleteButton);

        scrollPaneHolder.add(ingredientsScrollPane);
        scrollPaneHolder.add(wayOfMakingScrollPane);

        layout.weighty = 0.05;
        layout.weightx = 1.0;
        layout.fill = GridBagConstraints.BOTH;
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(buttonHolder, layout);

        layout.weighty = 1.0;
        layout.gridy = 1;
        this.add(scrollPaneHolder, layout);

        this.setVisible(true);
    }

    public void onNameChanged() {
        this.setTitle(recipe.getName());
    }

    private void onDeleteButtonClicked() {
        data.deleteRecipe(recipe.getId());
        window.onSearch();
        this.dispose();
    }

    public void renderText() {
        ingredientsLabel.setText(String.format("<html><div style='padding: 0 20px;'>Ingredients :<br>%s</div></html>", recipe.getIngredients().replace("\n", "<br>")));
        wayOfMakingLabel.setText(String.format("<html><div style='padding: 0 20px;'>Way of making :<br>%s</div></html>", recipe.getWayOfMaking().replace("\n", "<br>")));
    }
}

class Window extends JFrame {
    private final Data data = new Data();
    private final JTextField searchBar;
    private final JComboBox<String> types;
    private final ArrayList<JButton> results = new ArrayList<>();
    private final JPanel resultHolder;

    public Window() {
        super();

        // setting attributes to the window
        this.setTitle("Recipe app");
        this.setSize(500, 500);
        this.setLayout(new GridBagLayout());
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                data.saveData();
                System.exit(0);
            }
        });

        // creating the widgets
        GridBagConstraints layout = new GridBagConstraints();
        JButton abbButton = new JButton("Add Recipe");
        searchBar = new JTextField();
        types = new JComboBox<>();
        JPanel searchArea = new JPanel();
        resultHolder = new JPanel();
        JScrollPane scrollPane = new JScrollPane(resultHolder);

        // setting attributes and connecting the widgets
        abbButton.addActionListener(e -> new AddRecipeWindow(Window.this, Window.this.data));
        searchBar.addActionListener(e -> Window.this.onSearch());
        String[] _types = {"appetizers", "beverages", "main courses", "salads", "sandwiches", "soups and stews", "all"};
        for (String type : _types) {
            types.addItem(type);
        }
        searchArea.setLayout(new GridLayout(1, 2));
        resultHolder.setLayout(new GridBagLayout());
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        scrollPane.setPreferredSize(new Dimension(300, 300));
        scrollPane.setBorder(null);

        // adding the widgets
        layout.gridx = 0;
        layout.gridy = 0;
        this.add(abbButton, layout);


        searchArea.add(searchBar);
        searchArea.add(types);

        layout.gridy = 1;
        layout.insets = new Insets(10, 10, 10, 10);
        this.add(searchArea, layout);

        layout.gridy = 2;
        this.add(scrollPane, layout);
    }


    public void onSearch() {

        for (JButton result : results) {
            resultHolder.remove(result);
        }
        results.clear();

        String text = searchBar.getText().strip().toLowerCase();
        String type = (String) types.getSelectedItem();
        GridBagConstraints layout = new GridBagConstraints();
        layout.gridx = 0;
        layout.gridy = 0;
        layout.weightx = 1;
        layout.fill = GridBagConstraints.HORIZONTAL;

        for (Recipe recipe : data) {
            if (Objects.equals(type, "all")) {
                if (recipe.getName().strip().toLowerCase().contains(text)) {
                    JButton result = new JButton(recipe.getName());
                    results.add(result);

                    result.addActionListener(e -> new RecipeWindowInfo(Window.this, Window.this.data, recipe));

                    resultHolder.add(result, layout);
                    layout.gridy++;
                }
            } else {
                if (recipe.getName().strip().toLowerCase().contains(text) && Objects.equals(recipe.getType(), type)) {
                    JButton result = new JButton(recipe.getName());
                    results.add(result);

                    result.addActionListener(e -> new RecipeWindowInfo(Window.this, Window.this.data, recipe));

                    resultHolder.add(result, layout);
                    layout.gridy++;
                }
            }
        }
        resultHolder.revalidate();
        resultHolder.repaint();
    }
}

class Main {
    public static void main(String[] args) {
        Window window = new Window();
        window.setVisible(true);
    }
}