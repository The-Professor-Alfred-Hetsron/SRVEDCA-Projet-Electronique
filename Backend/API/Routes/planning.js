const express = require('express')
const router = express.Router()

const PlanningController = require('../Controllers/PlanningController')

router.post('/store', PlanningController.store)
router.get ('/', PlanningController.showAll)
router.put ('/update', PlanningController.update)
router.delete('/delete/:id',PlanningController.destroy)

module.exports = router