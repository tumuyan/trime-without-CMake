package com.osfans.trime.ime.broadcast

import com.osfans.trime.core.RimeNotification.OptionNotification
import com.osfans.trime.ime.dependency.InputScope
import java.util.concurrent.ConcurrentLinkedQueue

@InputScope
class InputBroadcaster : InputBroadcastReceiver {
    private val receivers = ConcurrentLinkedQueue<InputBroadcastReceiver>()

    fun <T> addReceiver(receiver: T) {
        if (receiver is InputBroadcastReceiver && receiver !is InputBroadcaster) {
            receivers.add(receiver)
        }
    }

    fun <T> removeReceiver(receiver: T) {
        if (receiver is InputBroadcastReceiver && receiver !is InputBroadcaster) {
            receivers.remove(receiver)
        }
    }

    fun clear() {
        receivers.clear()
    }

    override fun onRimeOptionUpdated(value: OptionNotification.Value) {
        receivers.forEach { it.onRimeOptionUpdated(value) }
    }
}
