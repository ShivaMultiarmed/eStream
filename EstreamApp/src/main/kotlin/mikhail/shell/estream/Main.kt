package mikhail.shell.estream

import io.ktor.network.selector.*
import io.ktor.network.sockets.*
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import java.math.BigInteger
import java.security.MessageDigest
import java.util.*
import java.util.concurrent.CountDownLatch

@OptIn(DelicateCoroutinesApi::class)
fun main() = runBlocking {
    val latch = CountDownLatch(1)
    val K = BigInteger("43327941536451757547021212229086144792243993750900499115474182045548247479320").toByteArray().hash()
    val IV = BigInteger.probablePrime(1024, Random()).toByteArray().hash()
    val job = GlobalScope.launch {
        val socket = aSocket(SelectorManager()).tcp().connect("localhost", 8000)
        try {
            val input = socket.openReadChannel()
            val output = socket.openWriteChannel(autoFlush = true)
            launch {
                while (true) {
                    val data = readln().encodeToByteArray()
                    output.writeFully(K, 0, K.size)
                    output.writeFully(IV, 0, IV.size)
                    output.writeFully(data, 0, data.size)
                    val encryptedData = ByteArray(data.size)
                    input.readAvailable(encryptedData, 0, encryptedData.size)
                    val hash = encryptedData.decodeToString()
                    println(hash)
                }
            }
        } catch (e: Exception) {
            println(e.stackTraceToString())
        }
    }
    latch.await()
}

fun ByteArray.hash(): ByteArray {
    val md = MessageDigest.getInstance("SHA-256")
    val hash = md.digest(this)
    return hash.copyOf(16)
}