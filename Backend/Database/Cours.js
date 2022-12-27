const mongoose = require('mongoose')

const coursSchema = new mongoose.Schema({
    code: {
        type: String,
        required: true,
        unique: true,
        minLength: 3,
        uppercase: true,
    },
    nom: {
        type: String,
        required: true,
    },
    classe: {
        type: mongoose.SchemaTypes.ObjectId,
        required: true,
        ref: 'Classe'
    },
    semestre: {
        type: String,
        required: true,
    },
    
})

module.exports = mongoose.model('Cours', coursSchema)
