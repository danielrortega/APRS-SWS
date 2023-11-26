  <h1>Estação Meteorológica APRS com ESP8266 e Integração com o Weather Underground</h1>

  <p>Este repositório contém o código para uma estação meteorológica simples construída usando o microcontrolador ESP8266. A estação meteorológica é equipada com sensores para medir temperatura, umidade e pressão atmosférica. Os dados coletados são enviados para a rede APRS e para o serviço Weather Underground.</p>

  <h2>Recursos:</h2>

  <ol>
    <li><strong>Integração de Sensores:</strong>
      <ul>
        <li>A estação meteorológica utiliza um sensor DHT22 para medições de temperatura e umidade.</li>
        <li>A pressão atmosférica é medida usando o sensor Adafruit BMP180.</li>
      </ul>
    </li>
    <li><strong>Conectividade de Rede:</strong>
      <ul>
        <li>O ESP8266 se conecta à rede Wi-Fi local usando a biblioteca WiFiManager para uma configuração fácil.</li>
        <li>Utiliza as bibliotecas ESP8266WiFi, ESP8266HTTPClient e WiFiUDP para comunicação em rede.</li>
      </ul>
    </li>
    <li><strong>Sincronização de Horário:</strong>
      <ul>
        <li>O tempo é sincronizado com um servidor NTP (Network Time Protocol) usando a biblioteca NTPClient.</li>
        <li>A estação é configurada para atualizar seu horário regularmente, garantindo uma marcação de dados precisa.</li>
      </ul>
    </li>
    <li><strong>Transmissão de Dados Meteorológicos:</strong>
      <ul>
        <li>A estação envia dados meteorológicos usando uma conexão TCP/IP para o servidor APRS (Automatic Packet Reporting System) em aprs.pu3dro.com.br.</li>
        <li>Os dados incluem temperatura, umidade e pressão atmosférica em um formato específico compatível com o APRS. Para ver os dados da minha estação acesse: <a href="https://aprs.fi/info/a/PU3DRO-13">https://aprs.fi/info/a/PU3DRO-13</a></li>
      </ul>
    </li>
    <li><strong>Integração com o Weather Underground:</strong>
      <ul>
        <li>Os dados de temperatura, umidade, ponto de orvalho e pressão atmosférica são enviados para o Weather Underground para armazenamento online e visualização.</li>
        <li>A integração é realizada por meio de requisições HTTP usando a biblioteca ESP8266WiFi.</li>
      </ul>
    </li>
    <li><strong>Reset Diário Automático:</strong>
      <ul>
        <li>O ESP8266 é programado para reiniciar diariamente em um horário especificado (resetHour) para garantir uma operação estável.</li>
      </ul>
    </li>
    <li><strong>Atualizações OTA (Over-the-Air):</strong>
      <ul>
        <li>A estação suporta atualizações OTA, permitindo o envio de atualizações de firmware sem fio.</li>
      </ul>
    </li>
  </ol>

  <h2>Uso:</h2>

  <ol>
    <li><strong>Configuração:</strong>
      <ul>
        <li>Modifique as variáveis <code>wundergroundID</code> e <code>wundergroundPassword</code> com o ID e senha da sua estação Weather Underground.</li>
        <li>Ajuste a variável <code>resetHour</code> para definir o horário desejado para o reset diário.</li>
        <li>Modifique as variáveis <code>aprsCallsignPrefix</code>, <code>aprsCallsignSSID</code>, <code>aprsPassword</code>, <code>aprsVersion</code>, <code>aprsLatitude</code> e <code>aprsLongitude</code> para configurar seu indicativo APRS, senha e coordenadas geográficas.</li>
      </ul>
    </li>
    <li><strong>Upload:</strong>
      <ul>
        <li>Faça o upload do código para a sua placa ESP8266 usando o Arduino IDE ou outro ambiente de desenvolvimento compatível.</li>
      </ul>
    </li>
    <li><strong>Configuração Inicial:</strong>
      <ul>
        <li>Ligue a estação meteorológica, e ela criará uma rede Wi-Fi chamada "APRS WeatherStation".</li>
        <li>Conecte-se a esta rede usando um dispositivo, e um portal de configuração será aberto para a configuração Wi-Fi.</li>
      </ul>
    </li>
    <li><strong>Monitoramento de Saída:</strong>
      <ul>
        <li>Abra o Monitor Serial no Arduino IDE para visualizar informações de depuração e garantir o correto funcionamento da estação meteorológica.</li>
      </ul>
    </li>
    <li><strong>Weather Underground:</strong>
      <ul>
        <li>Certifique-se de que sua estação Weather Underground está configurada com o ID e a senha corretos.</li>
        <li>Os dados da estação meteorológica serão enviados para o Weather Underground para rastreamento online e análise.</li>
      </ul>
    </li>
  </ol>

  <h2>Dependências:</h2>

<ul>
    <li><a href="https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi">ESP8266WiFi</a></li>
    <li><a href="https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient">ESP8266HTTPClient</a></li>
    <li><a href="https://github.com/adafruit/DHT-sensor-library">DHT</a></li>
    <li><a href="https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA">ArduinoOTA</a></li>
    <li><a href="https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src">WiFiUdp</a></li>
    <li><a href="https://github.com/arduino-libraries/NTPClient">NTPClient</a></li>
    <li><a href="https://github.com/PaulStoffregen/Time">TimeLib</a></li>
    <li><a href="https://github.com/tzapu/WiFiManager">WiFiManager</a></li>
    <li><a href="https://github.com/adafruit/Adafruit_BMP085_Unified">Adafruit_BMP085</a></li>
</ul>

  <h2>Notas:</h2>

  <ul>
    <li>Certifique-se de manipular suas credenciais do Weather Underground com segurança.</li>
    <li>O código está configurado para o meu servidor APRS (aprs.pu3dro.com.br). Ajuste os detalhes do servidor conforme necessário para outras regiões.</li>
    <li>Sinta-se à vontade para contribuir, relatar problemas ou personalizar o código conforme suas necessidades específicas!</li>
  </ul>
