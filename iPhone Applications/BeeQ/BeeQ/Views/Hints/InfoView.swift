//
//  InfoView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct InfoView: View {
    @EnvironmentObject var engine: BeeQEngine
    var body: some View {
            VStack () {
                Form {
                    Section (header: Text("Summary")) {
                        Text("Total Possible Words: \(engine.currentGame.totalPossibleWords)")
                        Text("Total Possible Pangrams: \(engine.currentGame.possiblePangrams)")
                        Text("Total Possible Points: \(engine.currentGame.totalPossiblePoints)")
                    }
                    ForEach(engine.currentGame.characterPossible, id: \.self) {count in StartCharacterView(count: count)}
                }
                DismissSheetView()
            }
    }
}

struct InfoView_Previews: PreviewProvider {
    static var previews: some View {
        InfoView()
    }
}
