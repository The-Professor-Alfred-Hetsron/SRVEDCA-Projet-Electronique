const express = require('express')
const router = express.Router()

const PresenceController = require('../Controllers/PresenceController')

router.get('/', PresenceController.getPresences)
router.post('/save', PresenceController.savePresence)
router.post('/saveMany', PresenceController.saveManyPresences)
router.delete('/delete', PresenceController.deletePresence)


module.exports = router