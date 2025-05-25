<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Update OLED Message</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      background: #121212;
      color: #ffffff;
      font-family: 'Segoe UI', sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
    }
    .form-container {
      background: #1f1f1f;
      padding: 30px;
      border-radius: 12px;
      box-shadow: 0 0 20px rgba(255, 255, 255, 0.05);
      width: 90%;
      max-width: 400px;
    }
    h2 {
      text-align: center;
      margin-bottom: 20px;
    }
    input[type="text"] {
      width: 100%;
      padding: 12px;
      font-size: 16px;
      border: none;
      border-radius: 6px;
      margin-bottom: 20px;
      background: #2e2e2e;
      color: #fff;
    }
    input[type="submit"] {
      width: 100%;
      padding: 12px;
      font-size: 16px;
      border: none;
      background: #4CAF50;
      color: white;
      border-radius: 6px;
      cursor: pointer;
    }
    input[type="submit"]:hover {
      background: #45a049;
    }
  </style>
</head>
<body>
  <div class="form-container">
    <h2>Send Message to ESP32</h2>
    <form action="send.php" method="POST">
      <input type="text" name="message" placeholder="Type message..." required>
      <input type="submit" value="Send to OLED">
    </form>
  </div>
</body>
</html>
