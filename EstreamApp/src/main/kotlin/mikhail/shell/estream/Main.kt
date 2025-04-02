import androidx.compose.desktop.ui.tooling.preview.Preview
import androidx.compose.material.Button
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.window.Window
import androidx.compose.ui.window.application
import mikhail.shell.estream.Encryptor


fun main() {
    val testKey = "test key".toCharArray().toTypedArray()
    val testInitVector = "test iv".toCharArray().toTypedArray()
    val testData = "Some secret info".toCharArray().toTypedArray()
    val hexResult = "c17920e1a4944ab5718fb9aa080f3aadf4491a6ddfb946a3344a5c79d55a7939fc42d9ee6df52d120cf544f61e96aed162cc9d556f46ac62a8cbd360a199a56ff6f695d0a4f7fe2d294f7686fbf5a6569ba409ca785f21f7303fbe72b13d3573f"
    val encryptor = Encryptor.instance
    val encryptedData = encryptor.encrypt(testData, testKey, testInitVector)
    println(encryptedData)
}
