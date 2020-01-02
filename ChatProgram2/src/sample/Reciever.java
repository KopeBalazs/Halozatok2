package sample;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;


/**
 * Created by pusok on 12/5/2018.
 */
public class Reciever implements Runnable{

    Socket socket;
    String recievedMessage;
    String threadName;
    Thread thread;

    javafx.scene.control.TextArea textArea;
    javafx.scene.control.ListView listView;
    private ObservableList userList = FXCollections.observableArrayList();
    public Reciever(Socket socket, String threadName) {
        this.socket = socket;
        this.threadName = threadName;
    }

    public Reciever(Socket socket, String threadName,javafx.scene.control.TextArea textArea, javafx.scene.control.ListView listView) {
        this.socket = socket;
        this.threadName = threadName;
        this.textArea = textArea;
        this.listView = listView;
    }

    public Socket getSocket() {
        return socket;
    }

    public void setSocket(Socket socket) {
        this.socket = socket;
    }

    public String getRecievedMessage() {
        return recievedMessage;
    }

    public void setRecievedMessage(String recievedMessage) {
        this.recievedMessage = recievedMessage;
    }


    @Override
    public void run()
    {
        InputStream input = null;
        BufferedReader reader;
        while(true)
        {
            try {
                input = socket.getInputStream();
                reader = new BufferedReader(new InputStreamReader(input));
                String line = reader.readLine();
                if (line.contains("USR")) {
                    userList.addAll(line.substring(0,line.length()-3));
                    listView.setItems(userList);
                }else {
                    textArea.appendText("Recive: " + line + "\n");
                }
                System.out.println("Message from server: " + line);
                recievedMessage = line;
            } catch (IOException e) {
                System.out.println("Failed to RECIEVE the message from server!!!");
                break;
            }
        }
    }

    public void start()
    {
        System.out.println("Starting: "+threadName);
        if (thread == null)
        {
            thread = new Thread(this, threadName);
            thread.start();
        }
    }
}
