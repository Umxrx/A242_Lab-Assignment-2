<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $message = $_POST["message"];

    $url = "https://my-project-id-default-rtdb.asia-southeast1.firebasedatabase.app/oled_text.json";

    $data = json_encode($message);

    $options = [
        "http" => [
            "header" => "Content-type: application/json\r\n",
            "method" => "PUT", // Use PUT to replace value
            "content" => $data
        ]
    ];
    $context = stream_context_create($options);
    $result = file_get_contents($url, false, $context);

    if ($result === FALSE) {
        die("Error sending to Firebase.");
    }

    header("Location: index.php");
    exit;
}
?>
