package org.example;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.*;

public class Main {
    private static final String AUTHOR_KEY = "BGjW9C6BkVrAyiyIK9gGihC/aiVZpACBsNKeW1dvXifY3jCbAqO5AvydhSum7X+3c86hRjvl/T/7OUpoSN1OOg==";
    private static final String VERSION = "2022-05-01";
    private static final String USER_ID = "376681";

    public static void main(String[] args) {
        try {
            // 连接到PostgreSQL数据库
            Connection connection = DriverManager.getConnection("jdbc:postgresql://8.130.146.112:5432/postgres", "postgres", "0");

            // 生成Token
            String token = createToken(AUTHOR_KEY, USER_ID, VERSION);

            // 构建URL
            URL url = new URL("https://iot-api.heclouds.com/thingmodel/query-device-property?product_id=CFcMIQYDRa&device_name=D1");

            while(true) {
                // 创建 HttpURLConnection 对象
                HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();

                // 设置请求方法为GET
                httpURLConnection.setRequestMethod("GET");

                // 设置请求头
                httpURLConnection.setRequestProperty("authorization", token);

                // 发送GET请求
                int responseCode = httpURLConnection.getResponseCode();

                // 打印响应状态码
                System.out.println("Response Code: " + responseCode);

                // 读取响应内容
                BufferedReader in = new BufferedReader(new InputStreamReader(httpURLConnection.getInputStream()));
                String inputLine;
                StringBuilder response = new StringBuilder();

                while ((inputLine = in.readLine()) != null) {
                    response.append(inputLine);
                }
                in.close();

                // 打印响应内容
                System.out.println("Response Body: " + response);

                // 将响应内容解析为对象列表
                List<SensorData> sensorDataList = parseResponse(response.toString());

                // 将数据插入到PostgreSQL数据库中
                insertDataIntoDatabase(sensorDataList, connection);

                // 关闭连接
                httpURLConnection.disconnect();
                Thread.sleep(3000); // 每3秒循环一次
            }
        } catch (IOException | NoSuchAlgorithmException | InvalidKeyException | SQLException | InterruptedException e) {
            e.printStackTrace();
        }
    }

    // 生成Token的函数
    public static String createToken(String authorKey, String userId, String version) throws NoSuchAlgorithmException, InvalidKeyException, UnsupportedEncodingException {
        String res = "userid/" + userId;
        long et = (System.currentTimeMillis() + 365 * 24 * 3600 * 1000) / 1000;
        String method = "sha1";

        String key = et + "\n" + method + "\n" + res + "\n" + version;

        // 使用HmacSHA1算法生成签名
        Mac sha1Hmac = Mac.getInstance("HmacSHA1");
        SecretKeySpec secretKey = new SecretKeySpec(Base64.getDecoder().decode(authorKey), "HmacSHA1");
        sha1Hmac.init(secretKey);
        byte[] signBytes = sha1Hmac.doFinal(key.getBytes(StandardCharsets.UTF_8));
        String sign = Base64.getEncoder().encodeToString(signBytes);

        // URL编码
        try {
            res = java.net.URLEncoder.encode(res, StandardCharsets.UTF_8.toString());
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException(e);
        }
        try {
            sign = java.net.URLEncoder.encode(sign, StandardCharsets.UTF_8.toString());
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException(e);
        }

        // 构造Token
        return "version=" + version + "&res=" + res + "&et=" + et + "&method=" + method + "&sign=" + sign;
    }

    // 解析响应内容为传感器数据对象列表的方法
    private static List<SensorData> parseResponse(String responseBody) {
        List<SensorData> sensorDataList = new ArrayList<>();

        try {
            ObjectMapper objectMapper = new ObjectMapper();
            JsonNode root = objectMapper.readTree(responseBody);

            // 获取data字段对应的数组
            JsonNode dataArray = root.get("data");

            // 创建一个Map用于存储每个时间点的SensorData对象
            Map<Long, SensorData> sensorDataMap = new HashMap<>();

            // 遍历数组
            for (JsonNode data : dataArray) {
                // 检查是否存在 "time" 字段
                JsonNode timeNode = data.get("time");
                if (timeNode != null) {
                    long time = timeNode.asLong();

                    // 获取或创建该时间点对应的SensorData对象
                    SensorData sensorData = sensorDataMap.getOrDefault(time, new SensorData(time));

                    // 获取identifier字段
                    String identifier = data.get("identifier").asText();

                    // 获取value字段
                    String value = data.has("value") ? data.get("value").asText() : null;

                    // 根据identifier字段判断是否为气体含量数据
                    switch (identifier) {
                        case "CH4":
                            sensorData.setCh4Value(data.get("value").asDouble());
                            break;
                        case "CO":
                            sensorData.setCoValue(data.get("value").asDouble());
                            break;
                        case "CO2":
                            sensorData.setCo2Value(data.get("value").asDouble());
                            break;
                        case "H2":
                            sensorData.setH2Value(data.get("value").asDouble());
                            break;
                        case "alarm_info":
                            // 如果identifier是alarm_info，设置flag和info
                            if ("NONE".equals(value)) {
                                sensorData.setFlag(false);
                                sensorData.setInfo(null);
                            } else {
                                sensorData.setFlag(true);
                                if(Objects.equals(value, "CH4")){
                                    sensorData.setInfo("甲烷浓度超标");
                                }else if(Objects.equals(value, "H2")){
                                    sensorData.setInfo("氢气浓度超标");
                                }else if(Objects.equals(value, "CO2")){
                                    sensorData.setInfo("二氧化碳浓度超标");
                                }else if(Objects.equals(value, "CO")){
                                    sensorData.setInfo("一氧化碳浓度超标");
                                }

                            }
                            break;
                    }

                    // 将更新后的SensorData对象放入Map中
                    sensorDataMap.put(time, sensorData);
                }
            }
            // 将Map中的所有SensorData对象添加到列表中
            sensorDataList.addAll(sensorDataMap.values());
        } catch (IOException e) {
            e.printStackTrace();
        }

        return sensorDataList;
    }


    // 将数据插入到PostgreSQL数据库中的方法
    private static void insertDataIntoDatabase(List<SensorData> sensorDataList, Connection connection) throws SQLException {
        String query = "INSERT INTO sensor_data (co2_data, ch4_data, co_data, h2_data, time, flag, info) VALUES (?, ?, ?, ?, ?, ?, ?)";
        PreparedStatement preparedStatement = connection.prepareStatement(query);


        for (SensorData sensorData : sensorDataList) {
            preparedStatement.setDouble(1, sensorData.getCo2Value());
            preparedStatement.setDouble(2, sensorData.getCh4Value());
            preparedStatement.setDouble(3, sensorData.getCoValue());
            preparedStatement.setDouble(4, sensorData.getH2Value());
            preparedStatement.setObject(5, sensorData.getTime());
            preparedStatement.setBoolean(6, sensorData.getFlag());
            preparedStatement.setString(7, sensorData.getInfo());
            preparedStatement.addBatch();
        }

        preparedStatement.executeBatch();
    }

    // 定义传感器数据类
    public static class SensorData {
        private static long time;
        private double ch4Value;
        private double coValue;
        private double co2Value;
        private double h2Value;
        private boolean flag;
        private String info;

        public SensorData(long time) {
            this.time = time;
        }

        public LocalDateTime getTime() {
            Instant instant = Instant.ofEpochMilli(time);
            LocalDateTime commonTime = LocalDateTime.ofInstant(instant, ZoneId.systemDefault());
            return commonTime;
        }

        public double getCh4Value() {
            return ch4Value;
        }

        public void setCh4Value(double ch4Value) {
            this.ch4Value = ch4Value;
        }

        public double getCoValue() {
            return coValue;
        }

        public void setCoValue(double coValue) {
            this.coValue = coValue;
        }

        public double getCo2Value() {
            return co2Value;
        }

        public void setCo2Value(double co2Value) {
            this.co2Value = co2Value;
        }

        public double getH2Value() {
            return h2Value;
        }

        public void setH2Value(double h2Value) {
            this.h2Value = h2Value;
        }

        public boolean getFlag() {
            return flag;
        }

        public void setFlag(boolean flag) {
            this.flag = flag;
        }

        public String getInfo() {
            return info;
        }

        public void setInfo(String info) {
            this.info = info;
        }
    }
}
