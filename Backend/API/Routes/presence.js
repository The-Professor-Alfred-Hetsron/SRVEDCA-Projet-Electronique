const express = require('express')
const router = express.Router()

const PresenceController = require('../Controllers/PresenceController')

router.get('/', PresenceController.getPresences)
router.post('/', PresenceController.getPresences) //J'ajoute cette ligne parce que l'ESP32 (et axios aussi) ne peut pas faire de GET ayant un body. Il fera donc un POST à la place
router.post('/save', PresenceController.savePresence)
router.post('/saveMany', PresenceController.saveManyPresences)
router.delete('/delete', PresenceController.deletePresence)


module.exports = router