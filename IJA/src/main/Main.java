/*
 * project: IJA 2020
 * authors: Jan Šulavík, Michal Polonec
 * content: Main class used for demonstration purposes in task 2
 *          of the IJA 2020 course on VUT FIT
 * */

package main;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.scene.layout.BorderPane;
import javafx.scene.control.Button;
import main.Controller;

import java.io.IOException;

public class Main extends Application {


    public static void main(String[] args) throws IOException {
        launch(args);

    }
        @Override
        public void start(Stage primaryStage) throws Exception {
            FXMLLoader loader = new FXMLLoader(getClass().getClassLoader().getResource("sample.fxml"));
            BorderPane root_element = loader.load();    // root element GUI

            Controller controller = loader.getController();

            primaryStage.setTitle("IJA application");
            Scene scene = new Scene(root_element);


            primaryStage.setScene(scene);

            primaryStage.show();

        }
}
