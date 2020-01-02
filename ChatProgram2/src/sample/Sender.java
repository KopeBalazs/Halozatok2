package sample;


import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Created by pusok on 12/5/2018.
 */
public class Sender implements Runnable{

    Socket socket;
    String sendMessage;
    String threadName;

    Thread thread;

    public Sender(Socket socket, String threadName, String message) {
        this.socket = socket;
        this.threadName = threadName;
        this.sendMessage = message;
    }

    @Override
    public void run() {
        OutputStream output = null;
        PrintWriter writer;
        try {
            output = socket.getOutputStream();
            writer = new PrintWriter(output, true);
            writer.println(sendMessage);
            System.out.println("Message to server: "+sendMessage);
        } catch (IOException e) {
            System.out.println("Failed to SEND the message from server!");
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
