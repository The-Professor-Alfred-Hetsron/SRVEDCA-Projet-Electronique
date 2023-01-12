const express = require('express')
const router = express.Router()
const { checkAdmin } = require("../Middleware/AuthMiddlewares")

const { getAllEtudiant, 
    getEtudiantByStatus, 
    updateSt,
    updateAllEtudiant,
    validated,deleteEtudiant} = require('../Controllers/EtudiantController')


    router.get('/', getAllEtudiant);
    router.get('/attente', getEtudiantByStatus);
    router.put('/update/:id', updateSt);
    router.patch('/validate/:id', validated);
    router.put('/updateAll',  updateAllEtudiant);
    router.delete('/delete/:id',  deleteEtudiant);

module.exports = router