package warehouse;

import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.util.StdConverter;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

@JsonIdentityInfo(generator = ObjectIdGenerators.PropertyGenerator.class, property = "id")
public class MyNode {
    private String id ;
    private ArrayList<MyNode> nextNodes;
    private MyCoordinate location;
    private boolean displayed;
    private boolean occupied;

    MyNode pathParent;      // used for finding path in the warehouse


    public MyNode(){}

    public MyNode(String id, ArrayList<MyNode> nextNodes, MyCoordinate location){
        this.id = id;
        nextNodes = new ArrayList<>();

        this.location = location;
        this.displayed = false;
        this.occupied= false;

    }

    public void setId(String id) {
        this.id = id;
    }

    public String getId() {
        return id;
    }

    public void setNextNodes(ArrayList<MyNode> nextNodes) {
        this.nextNodes = nextNodes;
    }

    public List<MyNode> getNextNodes() {
        return nextNodes;
    }

    public void setLocation(MyCoordinate location) {
        this.location = location;
    }

    public MyCoordinate getLocation() {
        return location;
    }

    public boolean isDisplayed() {
        return displayed;
    }

    public void setDisplayed(boolean displayed) {
        this.displayed = displayed;
    }

    public void setOccupied(boolean occupied) {
        this.occupied = occupied;
    }

    public boolean isOccupied() {
        return occupied;
    }

    public void setPathParent(MyNode pathParent) {
        this.pathParent = pathParent;
    }

    public MyNode getPathParent() {
        return pathParent;
    }


    @Override
    public String toString() {
        return "MyNode{" +
                "id=" + id +
                '}';
    }
}
