package main;

import com.fasterxml.jackson.databind.DeserializationConfig;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.util.JSONPObject;
import com.fasterxml.jackson.dataformat.yaml.YAMLFactory;
import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.util.StdConverter;
import com.fasterxml.jackson.dataformat.yaml.YAMLGenerator;
import gui.*;

import javafx.animation.PathTransition;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import javafx.scene.shape.Circle;
import warehouse.*;
import warehouse.MyCoordinate;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.geometry.Bounds;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.input.ScrollEvent;
import javafx.scene.layout.Region;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.text.TextAlignment;
import javafx.scene.transform.Scale;
import javafx.stage.Popup;
import javafx.stage.Stage;
import javafx.application.Platform;

import javafx.application.Application;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.scene.Group;
import javafx.scene.text.Text;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.VBox;
import javafx.scene.text.TextAlignment;
import javafx.scene.text.Font;
import javafx.stage.Stage;
import javafx.scene.shape.Rectangle;
import javafx.util.Duration;


import java.time.format.DateTimeFormatter;
import java.util.*;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.sql.Time;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.ResourceBundle;


import static java.time.temporal.ChronoUnit.MINUTES;

public class Controller implements Initializable {


    @FXML
    private Text clockText;

    @FXML
    private TextField setTimeTextField;

    @FXML
    private TextField setSpeedTextField;

    @FXML
    private Text setTimeWrongFormatText;

    @FXML
    private Text setSpeedWrongFormatText;

    @FXML
    private TextField setRequestGoodsTextField;

    @FXML
    private TextField setRequestQuantityTextField;



    @FXML
    public Pane MapPane;

    @FXML
    public ScrollPane scrollPane;

    @FXML
    public Button Update;

    @FXML
    public Button Request;

    @FXML
    public Button Blockade;

    @FXML
    public Button Ilustrate;

    private LocalTime currentTime = SimulationSettings.startTime;
    private List<Node> infoText = new ArrayList<Node>();

    int i = 0;

    YAMLFactory map_base = new YAMLFactory().disable(YAMLGenerator.Feature.WRITE_DOC_START_MARKER);
    ObjectMapper om = new ObjectMapper(map_base);


    Warehouse warehouse;

    {
        try {
            warehouse = om.readValue(new File("data/warehouse_base.yml"), Warehouse.class);
            warehouse.completeRequests();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Function creates HashMap of Pane which serve as visual of Shelfes storing goods,
     * also appends the Pane on the main pane which is second parameter
     * @param shelves of type ArrayList<> of elements Shelf
     * @param pane of type Pane - main layout Pane to which the created Shelfs are appended
     * @return mapPane of type HashMap<> with key Shelf1,Shelf2,...
     */

    public HashMap<Basket, Circle> BasketCircle(){
        HashMap<Basket, Circle> BasketCircle = new HashMap<>();

        BasketCircle.put(warehouse.getBaskets().get(0), new Circle(10, Color.rgb(180, 6, 6)));
        BasketCircle.get(warehouse.getBaskets().get(0)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(0).isClicked() == true){
                warehouse.getBaskets().get(0).setClicked(false);
            }else{warehouse.getBaskets().get(0).setClicked(true);}
        });

        BasketCircle.put(warehouse.getBaskets().get(1), new Circle(10, Color.rgb(191, 133, 95)));
        BasketCircle.get(warehouse.getBaskets().get(1)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(1).isClicked() == true){
                warehouse.getBaskets().get(1).setClicked(false);
            }else{warehouse.getBaskets().get(1).setClicked(true);}
        });
        BasketCircle.put(warehouse.getBaskets().get(2), new Circle(10, Color.rgb(101, 194, 58)));
        BasketCircle.get(warehouse.getBaskets().get(2)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(2).isClicked() == true){
                warehouse.getBaskets().get(2).setClicked(false);
            }else{warehouse.getBaskets().get(2).setClicked(true);}
        });
        BasketCircle.put(warehouse.getBaskets().get(3), new Circle(10, Color.rgb(14, 123, 65)));
        BasketCircle.get(warehouse.getBaskets().get(3)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(3).isClicked() == true){
                warehouse.getBaskets().get(3).setClicked(false);
            }else{warehouse.getBaskets().get(3).setClicked(true);}
        });
        BasketCircle.put(warehouse.getBaskets().get(4), new Circle(10, Color.rgb(1, 222, 202)));
        BasketCircle.get(warehouse.getBaskets().get(4)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(4).isClicked() == true){
                warehouse.getBaskets().get(4).setClicked(false);
            }else{warehouse.getBaskets().get(4).setClicked(true);}
        });
        BasketCircle.put(warehouse.getBaskets().get(5), new Circle(10, Color.rgb(91, 123, 215)));
        BasketCircle.get(warehouse.getBaskets().get(5)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(5).isClicked() == true){
                warehouse.getBaskets().get(5).setClicked(false);
            }else{warehouse.getBaskets().get(5).setClicked(true);}
        });
        BasketCircle.put(warehouse.getBaskets().get(6), new Circle(10, Color.rgb(6, 62, 132)));
        BasketCircle.get(warehouse.getBaskets().get(6)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(6).isClicked() == true){
                warehouse.getBaskets().get(6).setClicked(false);
            }else{warehouse.getBaskets().get(6).setClicked(true);}
        });
        BasketCircle.put(warehouse.getBaskets().get(7), new Circle(10, Color.rgb(81, 74, 193)));
        BasketCircle.get(warehouse.getBaskets().get(7)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(7).isClicked() == true){
                warehouse.getBaskets().get(7).setClicked(false);
            }else{warehouse.getBaskets().get(7).setClicked(true);}
        });
        BasketCircle.put(warehouse.getBaskets().get(8), new Circle(10, Color.rgb(100, 28, 147)));
        BasketCircle.get(warehouse.getBaskets().get(8)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(8).isClicked() == true){
                warehouse.getBaskets().get(8).setClicked(false);
            }else{warehouse.getBaskets().get(8).setClicked(true);}
        });
        BasketCircle.put(warehouse.getBaskets().get(9), new Circle(10, Color.rgb(255, 217, 20)));
        BasketCircle.get(warehouse.getBaskets().get(9)).setOnMouseClicked((MouseEvent e) -> {
            if(warehouse.getBaskets().get(9).isClicked() == true){
                warehouse.getBaskets().get(9).setClicked(false);
            }else{warehouse.getBaskets().get(9).setClicked(true);}
        });

        return BasketCircle;
    }

    /**
     * Creates Hashmap for representing Nodes on the Pane view
     * @param Pane
     * @return
     */
    public HashMap<MyNode, Circle> nodesMap(Pane Pane){
        HashMap<MyNode, Circle> nodesMap = new HashMap<>();

        for(MyNode node : warehouse.getNodes()){
            nodesMap.put(node, new Circle(5, Color.BLACK));
            nodesMap.get(node).setLayoutX(node.getLocation().getCoord_x());
            nodesMap.get(node).setLayoutY(node.getLocation().getCoord_y());
            nodesMap.get(node).setVisible(false);
            Pane.getChildren().add(nodesMap.get(node));
        }
       return nodesMap;
    }

    /**
     * Creates Hashmap to represent shelves on the Pane view
     * @param shelves
     * @param mapa
     * @return
     */
    public HashMap<String, Pane> mapPane(ArrayList<Shelf> shelves, Pane mapa){
        HashMap<String, Pane> mapPane = new HashMap<String,Pane>();
        for(Shelf shelf : shelves){
            mapPane.put("Shelf" + shelf.getId(), new Pane());
            mapPane.get("Shelf" + shelf.getId()).setPrefSize(75,50);
            mapPane.get("Shelf" + shelf.getId()).setStyle("-fx-background-color: green");
            mapPane.get("Shelf" + shelf.getId()).setLayoutX(shelf.getPosition().getCoord_x());
            mapPane.get("Shelf" + shelf.getId()).setLayoutY(shelf.getPosition().getCoord_y());
            mapa.getChildren().add(mapPane.get("Shelf" + shelf.getId()));
        }
        return mapPane;

    }

    /**
     * Holds info about what is inside of the Shelf
     * @param shelves
     * @return
     */

    public HashMap<String, Pane> popupPane (ArrayList<Shelf> shelves){
        HashMap<String, Pane> popupPane = new HashMap<String, Pane>();

        for (Shelf shelf : shelves){
            Text text = new Text();
            infoText.add(text);
            text.setFont(new Font(20));
            popupPane.put("Shelf" + shelf.getId(), new Pane());
            popupPane.get("Shelf" + shelf.getId()).setStyle("-fx-background-color: white");
            popupPane.get("Shelf" + shelf.getId()).setPrefSize( 200,125);
            text.setText("\nThis contains:" + shelf.storedGoods.name + "\n Weight:" + shelf.storedGoods.weight  + "\n of quantity:" + shelf.quantity);
            popupPane.get("Shelf" + shelf.getId()).getChildren().add(text);

        }
        return popupPane;
    }


    /**
     * Creates the popup stuff which holds the popupPane which holds the Shelf info
     * @param shelves
     * @param popupInfoPane
     * @return
     */

    public HashMap<String, Popup> mapPopup(ArrayList<Shelf> shelves, HashMap<String, Pane> popupInfoPane){
        HashMap<String, Popup> mapPopup = new HashMap<String,Popup>();

        for(Shelf shelf : shelves){
            mapPopup.put("Shelf" + shelf.getId(), new Popup());
            mapPopup.get("Shelf" + shelf.getId()).getContent().add(popupInfoPane.get("Shelf" + shelf.getId()));

        }

        return mapPopup;
    }

    /**
     * creates the hoverProperty and the functionality of actually display the info when you hover with cursor over the visible Shelf on warehouse map
     * @param shelves
     * @param mapPane
     * @param popupPane
     * @param mapPopup
     */

    public void createHoverPop (ArrayList<Shelf> shelves, HashMap<String, Pane> mapPane, HashMap<String, Pane> popupPane , HashMap<String, Popup> mapPopup) {
        for (Shelf shelf : shelves) {
            mapPane.get("Shelf" + shelf.getId()).hoverProperty().addListener((obs, oldVal, newValue) -> {
                if (newValue) {
                    Bounds bnds = mapPane.get("Shelf" + shelf.getId()).localToScreen(mapPane.get("Shelf" + shelf.getId()).getLayoutBounds());
                    double x = bnds.getMinX() - (popupPane.get("Shelf" + shelf.getId()).getWidth()) + (popupPane.get("Shelf" + shelf.getId()).getWidth());
                    double y = bnds.getMinY() - popupPane.get("Shelf" + shelf.getId()).getHeight();
                    mapPopup.get("Shelf" + shelf.getId()).show(mapPane.get("Shelf" + shelf.getId()), x, y);
                } else {
                    mapPopup.get("Shelf" + shelf.getId()).hide();
                }
            });
        }
    }

    /**
     * Function sets the position of the node representing Basket to new position from basket.nextNode
     * and calls UpdateNode to update both nodes according to the basket.pathNodes
     * @param basketCircleHashMap
     * @param nodesMap
     */
    public void Updateposition(HashMap<Basket, Circle> basketCircleHashMap, HashMap<MyNode, Circle> nodesMap){
        Platform.runLater(() ->{
            boolean visible = false;
            for(Basket basket : warehouse.getBaskets()){
                if(basket.isMoving() == true) {

                    if(basket.getCurrentNode().getLocation() != null ){
                        basketCircleHashMap.get(basket).setLayoutX(basket.getCurrentNode().getLocation().getCoord_x());
                        basketCircleHashMap.get(basket).setLayoutY(basket.getCurrentNode().getLocation().getCoord_y());

                        //if(basket.getCurrentNode() == ){
                        //    basket.getItem()
                        //}

                        basket.setNextNode(UpdateNode(basket, nodesMap));
                        basket.setCurrentNode(basket.getNextNode());
                    }


                    if(visible == false || basket.isClicked() == true) {
                        if (basket.isClicked() == true) {
                            makeVisible(basket, nodesMap);
                            visible = true;
                        } else {
                            makeInvisible(basket, nodesMap);
                            visible = false;
                        }
                    }

                }
            }
        });
    }

    /**
     * Takes basket and updates the nextNode to the following from basket.pathNode
     * @param basket
     * @return
     */

    public MyNode UpdateNode(Basket basket, HashMap<MyNode, Circle> nodesMap){
        MyNode nextNode = new MyNode();

        for(MyNode node: basket.getPathNodes()){
            if(basket.isParked() == true){
                nextNode = basket.getPathNodes().get(0);
                basket.setParked(false);

                return nextNode;
            }
            if(node.equals(basket.getCurrentNode())){
                int indexOf = basket.getPathNodes().indexOf(node);
                if (basket.getPathNodes().size() > indexOf + 1) {
                    nextNode = basket.getPathNodes().get(indexOf + 1);

                    return nextNode;
                }else{

                    }

                }
            }
            if(basket.getPathNodes().get(basket.getPathNodes().size() - 1) == basket.getCurrentNode() && basket.getGoalNodes().size() >= 1) {

                basket.getGoalNodes().remove(0);

                    if (basket.getGoalNodes() != null && basket.getGoalNodes().size() != 0) {
                        makeInvisible(basket, nodesMap);
                        basket.setPathNodes(basket.findPath(basket.getGoalNodes().get(0), this.warehouse));
                        basket.setCurrentNode(basket.getPathNodes().get(0));
                        return basket.getCurrentNode();

                    }

            }

        return nextNode;
    }

    /**
     * Function makes Basket path nodes visible(changes color to black)
     * @param basket
     */
    public void makeVisible(Basket basket, HashMap<MyNode, Circle> nodesMap){
        for(MyNode node : basket.getPathNodes()){
            nodesMap.forEach((key,value) ->{
                if (key.equals(node) == true) {

                    nodesMap.get(key).setVisible(true);
                }
            });
        }
    }

    /**
     * Function makes Basket path nodes invisible(changes color back to white)
     * @param basket
     */
    public void makeInvisible(Basket basket, HashMap<MyNode, Circle> nodesMap){
        for(MyNode node : basket.getPathNodes()){
            nodesMap.forEach((key,value) ->{
                if (key.equals(node) == true) {

                    nodesMap.get(key).setVisible(false);
                }
            });
        }

    }

    /**
     * Function updates the contents of each individual Shelf so there is always correct quantity
     * @param shelves
     * @param mapPane
     * @param popupPane
     * @param mapPopup
     */
    public void Update(ArrayList<Shelf> shelves, HashMap<String, Pane> mapPane, HashMap<String, Pane> popupPane , HashMap<String, Popup> mapPopup){
      Platform.runLater(() -> {
            for (Shelf shelf : shelves) {
                popupPane.get("Shelf" + shelf.getId()).getChildren().remove(infoText.remove(0));
            }

            for (Shelf shelf : shelves) {
                Text text = new Text();
                infoText.add(text);
                text.setFont(new Font(20));
                text.setText("\nThis contains:" + shelf.storedGoods.name + "\n Weight:" + shelf.storedGoods.weight + "\n of quantity:" + shelf.quantity);
                popupPane.get("Shelf" + shelf.getId()).getChildren().add(text);
            }
        });
    }


    /**
     * Is button which speeds up time
     */
    public void onSetSpeedBtnClick() {
        Platform.runLater(() -> {
            if(setSpeedTextField.getText().isEmpty()){
                return;
            }

            if(setSpeedTextField.getText().matches("\\d+x")){
                try {
                    SimulationSettings.speedRatio = Double.parseDouble(setSpeedTextField.getText().substring(0, setSpeedTextField.getText().length()-1));
                    SimulationSettings.updateIntervalMs = 1* (int)SimulationSettings.speedRatio;
                    setSpeedWrongFormatText.setVisible(false);
                }catch (Exception e){
                    setSpeedWrongFormatText.setVisible(true);
                }
            }else{
                setSpeedWrongFormatText.setVisible(true);
            }

            setSpeedTextField.clear();
        });
    }

    /**
     * Is button which sets time to a specific value
     */
    public void onSetTimeBtnClick() {
        Platform.runLater(() -> {
            if (setTimeTextField.getText().isEmpty()) {
                return;
            }

            try {
                currentTime = LocalTime.parse(setTimeTextField.getText(), DateTimeFormatter.ofPattern("HH:mm:ss"));
            } catch (Exception e) {
                try {
                    currentTime = LocalTime.parse(setTimeTextField.getText(), DateTimeFormatter.ofPattern("HH:mm"));
                } catch (Exception e2) {
                    setTimeWrongFormatText.setVisible(true);
                    setTimeTextField.clear();
                    return;
                }
            }

            setTimeTextField.clear();
            setTimeWrongFormatText.setVisible(false);

            SimulationSettings.startTime = currentTime;

        });
    }

    /**
     * Is button which will handle Manual Request
     */
    public void onSetRequestBtnClick(){
        Platform.runLater(()->{
            if(setRequestGoodsTextField.getText().isEmpty() || setRequestQuantityTextField.getText().isEmpty()){
                return;
            }

            setRequestGoodsTextField.clear();
            setRequestQuantityTextField.clear();
        });
    }



    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

        ZoomOperator zoom = new ZoomOperator();
        Pane mapa = new Pane();

        //creates zooming abbility on the Pane view
        scrollPane.setOnScroll(new EventHandler<ScrollEvent>() {
            @Override
            public void handle(ScrollEvent event) {
                double zoomFactor = 1.5;
                if (event.getDeltaY() <= 0) {
                    // zoom out
                    zoomFactor = 1 / zoomFactor;
                }
                zoom.zoom(scrollPane, zoomFactor, event.getSceneX(), event.getSceneY());
            }
        });
        // first the Shelf function which displays on map
        HashMap<String, Pane> mapaPane = mapPane(warehouse.getShelves(), mapa);

        //second the info Pane which holds the info
        HashMap<String, Pane> popupPane = popupPane(warehouse.getShelves());

        //third the Popup function to create the pop ups and append the info stuff

        HashMap<String, Popup> mapPopup = mapPopup(warehouse.getShelves(), popupPane);

        //and last but not least the actual displaying functionality

        createHoverPop(warehouse.getShelves(), mapaPane, popupPane, mapPopup);


        //Hashmap with Circles for nodes
        HashMap<MyNode, Circle> nodesMap = nodesMap(mapa);

        //Hashmap for Circles for Baskets
        HashMap<Basket, Circle> basketMap= BasketCircle();

        //hlavna view mapa
        MapPane = mapa;

        //adds Baskets from HashMap where key is basket and value Circle which will represent the basket
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(0)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(1)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(2)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(3)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(4)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(5)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(6)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(7)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(8)));
        MapPane.getChildren().add(basketMap.get(warehouse.baskets.get(9)));


        scrollPane.setContent(MapPane);



        //naloadujem baskety na Pane view
        warehouse.baskets.get(0).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(1).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(2).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(3).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(4).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(5).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(6).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(7).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(8).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
        warehouse.baskets.get(9).setCurrentNode(warehouse.getNodes().get(warehouse.NodeIndex("1001")));


        warehouse.completeRequests();

        warehouse.loadRequestToBasket();
        warehouse.checkRequestsSize();

        Timer timer = new Timer();
        /**
         * This is the main loop of the application
         */
        timer.schedule(new TimerTask() {
            @Override
            public void run() {

                for(Basket basket : warehouse.getBaskets()){
                    if (basket.isMoving() == true){


                        if(basket.isParked() == true){
                            basket.findGoalNodes(warehouse);
                            basket.getGoalNodes().add(warehouse.getNodes().get(warehouse.NodeIndex("1001")));
                            basket.setPathNodes(basket.findPath(basket.getGoalNodes().get(0), warehouse));
                        }

                    }
                }

                Update(warehouse.getShelves(), mapaPane, popupPane, mapPopup);

                Updateposition(basketMap, nodesMap);

                currentTime = currentTime.plusNanos((long) (SimulationSettings.updateIntervalMs*1000000*SimulationSettings.speedRatio));

            }

        }, 0, SimulationSettings.updateIntervalMs);

    }
}




