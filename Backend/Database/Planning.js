const mongoose = require('mongoose')
const moment = require('moment')
const {toMinutes} = require('./functions')

const planningSchema = new mongoose.Schema({
    jourSemaine: {
        type: Number,
        required: true,
        min: 1, //Pour Lundi
        max: 7, //Pour Dimanche
    },
    heureDebut: {
        type: Date,
        required: true,
    },
    heureFin: {
        type: Date,
        required: true,
    },
    cours: {
        type: mongoose.SchemaTypes.ObjectId,
        required: true,
        ref: 'Cours',
    },
    
})

planningSchema.virtual('hDebut').get(function(){
    return moment(this.heureDebut).format('HH:mm')
})

//Renvoie l'heure de début sous forme de nombre de minutes
planningSchema.virtual('hDebutNumber').get(function(){
    return toMinutes(this.heureDebut)
})

planningSchema.virtual('hFin').get(function(){
    return moment(this.heureFin).format('HH:mm')
})

//Renvoie l'heure de fin sous forme de nombre de minutes
planningSchema.virtual('hFinNumber').get(function(){
    return toMinutes(this.heureFin)
})


module.exports = mongoose.model('Planning', planningSchema)
