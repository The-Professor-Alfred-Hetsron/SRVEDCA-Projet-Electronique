const mongoose = require('mongoose')

const classeSchema = new mongoose.Schema({
    nom: {
        type: String,
        required: true,
        minLength: 3,
        uppercase: true,
    },
    salle: {
        type: String,
        uppercase: true,
    },
    
})

const Classe = mongoose.model('Classe', classeSchema)
module.exports = Classe
