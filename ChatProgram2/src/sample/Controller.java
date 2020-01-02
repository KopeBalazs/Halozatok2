package sample;

import javafx.event.ActionEvent;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;

import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.TimeUnit;

public class Controller {

    public TextArea messageTextArea;
    public Button connectButton;
    public TextField nameTextField;
    public TextField messageTextField;
    public Button sendButton;
    public ListView activeUsersListView;
    public TextField userNamesNumberTextField;

    Socket socket;
    String message="";


    public void connectButtonAction(ActionEvent actionEvent) {
        try {
            socket = new Socket("192.168.43.41",13000);
            System.out.println("Socket inicialized!");
            String name=nameTextField.getText();
            Sender sender = new Sender(socket,"Sender", name);
            sender.start();
            TimeUnit.SECONDS.sleep(1);
            Reciever reciever = new Reciever(socket,"Reciever",messageTextArea,activeUsersListView);
            reciever.start();
        } catch (IOException e) {
            System.out.println("Socket NOT inicialized!");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void sendButtonAction(ActionEvent actionEvent) {
        message = message+messageTextField.getText();
        Sender sender = new Sender(socket,"Sender", message);
        sender.start();
        messageTextArea.appendText("You: "+messageTextField.getText()+"\n");
        message="";
    }

    public void activeUsersButtonAction(ActionEvent actionEvent) {
        Sender sender = new Sender(socket,"Sender", "LSTUSRS");
        sender.start();
        activeUsersListView.getItems().clear();
    }

    public void activeUsersClickAction(MouseEvent mouseEvent) {
        String username = activeUsersListView.getSelectionModel().getSelectedItem().toString();
        message = message+username.length()+username;
        System.out.println(message);
    }

    public void sendAllButtonAction(ActionEvent actionEvent) {
        message = "LL";
    }

    public void privateMessageButtonAction(ActionEvent actionEvent) {
        message = "PRVTMSG";
    }

    public void groupMessageButtonAction(ActionEvent actionEvent) {
        message = "GRP"+userNamesNumberTextField.getText();
    }
}
