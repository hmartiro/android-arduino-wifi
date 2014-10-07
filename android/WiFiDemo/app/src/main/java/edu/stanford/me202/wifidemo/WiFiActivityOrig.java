package edu.stanford.me202.wifidemo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

public class WiFiActivityOrig extends Activity {

    private static final String TAG = "WifiConnect";
    private TextView IPAddress;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wi_fi);
        IPAddress = (TextView) findViewById(R.id.IPInput);

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);


        new FileServerAsyncTask(getApplicationContext(),
                (TextView) findViewById(R.id.incomingText))
                .execute();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        return true;
    }

    public static class FileServerAsyncTask extends AsyncTask<Void, Void, String> {

        private Context context;
        private TextView statusText;

        public FileServerAsyncTask(Context context, View statusText) {
            this.context = context;
            this.statusText = (TextView) statusText;
        }

        @Override
        protected String doInBackground(Void... params) {
            try {

                // Create a server socket and wait for client connections. This
                //  call blocks until a connection is accepted from a client
                ServerSocket serverSocket = new ServerSocket(8888);
                Log.v(TAG, "Server socket opened");
                Socket client = serverSocket.accept();
                Log.v(TAG, "Server socket connection accepted");

                // If this code is reached, a client has connected and
                //  transferred data.
                InputStream inputStream = client.getInputStream();
                OutputStream outputStream = client.getOutputStream();

                byte[] buffer = new byte[1024];
                byte[] bufferOut = new byte[1024];

                int byteCount = inputStream.read(buffer);
                String readMessage = new String(buffer, 0, byteCount);
                Log.v(TAG, "Server received: " + readMessage);
                String outMessage = "ME202 - Hey back!";
                bufferOut = outMessage.getBytes();
                outputStream.write(bufferOut);

                inputStream.close();
                outputStream.close();
                serverSocket.close();

                return readMessage;
            } catch (IOException e) {
                Log.e(TAG, e.getMessage());
                return null;
            }
        }

        @Override
        protected void onPostExecute(String result) {
            if (result != null) {
                statusText.setText(result);
            }
        }
    }

    public void wifiSend(View v) {
        EditText outgoingMessage = (EditText) findViewById(R.id.MsgOutput);
        String IPAddressString = IPAddress.getText().toString();
        clientConnection(IPAddressString, 80, outgoingMessage.getText().toString());
    }

    private void clientConnection(String host, int port, String outMsg) {

        Socket socket = new Socket();
        byte[] buffer = new byte[1024];
        long startTime, currentTime;
        long timeoutMs = 500;

        try {
            // Create a client socket with the host, port, and timeout information.
            Log.v(TAG, "socket.bind");
            socket.bind(null);
            Log.v(TAG, "InetSocketAddress");
            InetSocketAddress address = new InetSocketAddress(host, port);
            Log.v(TAG, "socket.connect");
            try {
                socket.connect(address);
            } catch (IOException io) {
                Log.v(TAG, "IO");
            } catch (IllegalArgumentException ia) {
                Log.v(TAG, "IA");
            }

            Log.v(TAG, "socket.getoutputstream");
            OutputStream outputStream = socket.getOutputStream();

            String outMsgWithCr = outMsg.concat("\n");	//add CR to facilitate handling for server and client
            buffer = outMsgWithCr.getBytes();
            outputStream.write(buffer);
            Log.v(TAG, "Sent " + outMsg);

            InputStream inputStream = socket.getInputStream();
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            startTime = System.currentTimeMillis();
            currentTime = startTime;
            while ((inputStream.available() == 0) && (currentTime - startTime < timeoutMs)) {
                currentTime = System.currentTimeMillis();
            }
            if(currentTime - startTime >= timeoutMs) {
                Log.v(TAG,"timeout waiting for InputStream");
            }

            String nextLine = null;
            Log.v(TAG, "before bufferedreader starts reading");
            nextLine = in.readLine();
            Log.v(TAG,nextLine);
            TextView incomingText = (TextView) findViewById(R.id.incomingText);
            incomingText.setText(nextLine);

            outputStream.close();
            inputStream.close();
        } catch (IOException e) {
            // catch logic
        }

        // Clean up any open sockets when done transferring or if an exception
        finally {
            if (socket != null) {
                if (socket.isConnected()) {
                    try {
                        socket.close();
                    } catch (IOException e) {
                        // catch logic
                    }
                }
            }
        }
    }
}
