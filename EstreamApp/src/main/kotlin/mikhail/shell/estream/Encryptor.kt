package mikhail.shell.estream

import com.sun.jna.Library
import com.sun.jna.Native

interface Encryptor: Library {
    fun encrypt(
        input: Array<Char>,
        key: Array<Char>,
        initVector: Array<Char>
    ): Array<Char>

    companion object {
        val instance by lazy {
            Native.load("hc128", Encryptor::class.java)
        }
    }
}